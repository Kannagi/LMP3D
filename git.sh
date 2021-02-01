#!/bin/sh
read message
echo \"$message\"

git rm git.sh
git commit -m "$message"
#git remote add origin https://github.com/Kannagi/LMP3D.git
#git push -u origin master
git push
