#! /bin/bash
SCRIPT=$(realpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
REPO_DIR=${SCRIPTPATH}/..

echo "We will increase fan speed every 3 seconds by 25 points"

p = 5
for i in {0..10}
do
    setting=$((p+i*25))
    echo Set speed to ${setting}
    ${REPO_DIR}/user/build/user write ${setting}
    fan_speed=`${REPO_DIR}/user/build/user read`
    if ["$fan_speed" -eq "$setting"] then
        echo Speed setting correctly applied
    else
        echo Speed setting isn't applied. Fan speed is ${fan_speed}
    fi
    sleep 3
done