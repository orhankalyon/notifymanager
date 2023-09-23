import os
import tomli
import requests
import re
import json
import sqlite3
from bs4 import BeautifulSoup

HOME = os.path.expanduser('~')
NM_CONFIG_DIR = os.getenv("NM_CONFIG_DIR", f"{HOME}/.config/notifymanager")


with open(f"{NM_CONFIG_DIR}/config.toml", mode="rb") as fp:
    config = tomli.load(fp)

ytInstance = config['youtube']['instance']
ytSubscriptions = config['youtube']['subscriptions']


connection = sqlite3.connect(f"{NM_CONFIG_DIR}/notifications.db")
cursor = connection.cursor()

cursor.execute(
    "CREATE TABLE IF NOT EXISTS "
    "youtube(id INTEGER PRIMARY KEY AUTOINCREMENT, channel_id TEXT, "
    "name TEXT, count INTEGER)"
)


for ytChannel in ytSubscriptions:
    response = requests.get(
        f"https://www.youtube.com/{ytChannel}/about",
        cookies={'CONSENT': 'YES+1'}
    )

    soup = BeautifulSoup(response.text, "html.parser")

    channelID = re.search(
        r'\{"key":"browse_id","value":"(.*?)"\}',
        str(soup)
    ).group(1)

    response = requests.get(
        f"{ytInstance}/api/v1/channels/videos/{channelID}"
    )
    data = response.json()

    channelName = data['videos'][0]['author']
    videoCount = 0

    while True:
        try:
            data['videos'][videoCount]['videoId']
            videoCount += 1
        except IndexError:
            break

    rowID = cursor.execute(
        "SELECT id FROM youtube WHERE channel_id = ?",
        (channelID,)
    ).fetchone()

    if rowID is None:
        cursor.execute(
            "INSERT INTO youtube(channel_id, name, count) VALUES(?, ?, ?)",
            (channelID, channelName, videoCount)
        )

        rowID = cursor.execute(
            "SELECT id FROM new WHERE name = ?",
            (channelName,)
        ).fetchone()

        if rowID is None:
            cursor.execute(
                "INSERT INTO new(name, count) "
                "VALUES(?, ?)",
                (channelName, videoCount)
            )
        else:
            cursor.execute(
                "UPDATE new SET count = ? WHERE name = ?",
                (videoCount, channelName)
            )
    else:
        rowVideoCount = cursor.execute(
            "SELECT count FROM youtube WHERE channel_id = ?",
            (channelID,)
        ).fetchone()

        newVideoCount = videoCount - rowVideoCount[0]

        if newVideoCount > 0:
            rowID = cursor.execute(
                "SELECT id FROM new WHERE name = ?",
                (channelName,)
            ).fetchone()

            if rowID is None:
                cursor.execute(
                    "INSERT INTO new(name, count) "
                    "VALUES(?, ?)",
                    (channelName, newVideoCount)
                )
            else:
                cursor.execute(
                    "UPDATE new SET count = ? WHERE name = ?",
                    (newVideoCount, channelName)
                )

        cursor.execute(
            "UPDATE youtube SET count = ? WHERE channel_id = ?",
            (videoCount, channelID)
        )

    connection.commit()
