reverse_ssh.service -> /etc/systemd/system/
-------------------------------------------

# This SSH Reverse Service
# By David Both
# Licensed under GPL V2
#



[Unit]
Description=SSH Reverse Service
Wants=reboot_ssh.timer



[Service]
Type=oneShot
ExecStart=/bin/bash /home/<user>/connct-to-cloud.sh



[Install]
WantedBy=multi-user.target


##########################################

reverse_ssh.timer -> /etc/systemd/system/
-----------------------------------------

# This timer unit is for testing
# By David Both
# Licensed under GPL V2
#



[Unit]
Description=Logs some system statistics to the systemd journal
Requires=reboot_ssh.service



[Timer]
Unit=reboot_ssh.service
OnCalendar=*-*-* *:*:00
Persistent=true


[Install]
WantedBy=timers.target


###########################################

connct-to-cloud.sh -> /home/<user>/
-----------------------------------

#!/bin/bash


USERNAME="<ssh.user>"
PRIVATE_KEY="/home/<user>/.ssh/<ssh.private.key>"
LOCAL_PORT="22"
REMOTE_PORT="2222"
CLOUD_SERVER_NAME="<cloud_ip>"


if [[ $(ps -ef | grep -c "/usr/bin/ssh -i $PRIVATE_KEY -nNT -R $REMOTE_PORT:localhost:$LOCAL_PORT$USERNAME@$CLOUD_SERVER_NAME") -eq 1 ]]; then
        logger -t $0 "Connecting to $CLOUD_SERVER_NAME as $USERNAME mapping remote port $REMOTE_PORT back to here on local port $LOCAL_PORT"
        logger -t $0 "USERNAME: $USERNAME"
        logger -t $0 "PRIVATE_KEY: $PRIVATE_KEY"
        logger -t $0 "LOCAL_PORT: $LOCAL_PORT"
        logger -t $0 "REMOTE_PORT: $REMOTE_PORT"
        logger -t $0 "CLOUD_SERVER_NAME: $CLOUD_SERVER_NAME"
        ssh -i $PRIVATE_KEY -nNT -R $REMOTE_PORT:localhost:$LOCAL_PORT $USERNAME@$CLOUD_SERVER_NAME -t $0
        if [[$? -eq 0]]; then
                logger -t $0 "SSH connection succeeded"
        else
                logger -t $0 "Connection did not succeed. Will try again in a minute. SSH connection status code: $?"
        fi
else
                logger -t $0 "Already connected to $CLOUD_SERVER_NAME as $USERNAME"
fi
