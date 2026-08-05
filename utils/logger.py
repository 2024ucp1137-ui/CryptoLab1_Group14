from datetime import datetime
import os

LOG_DIR = "outputs"
LOG_FILE = os.path.join(LOG_DIR, "activity.log")


def log_activity(message):
    os.makedirs(LOG_DIR, exist_ok=True)

    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    with open(LOG_FILE, "a") as file:
        file.write(f"[{timestamp}] {message}\n")