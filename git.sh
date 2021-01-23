#!/bin/sh
read message
echo \"$message\"
git add VU/
git add bin/
git add LMP3D/
git add *
git rm LMP3D_lib.cbp
git commit -m "$message"
#git remote add origin https://github.com/Kannagi/LMP3D.git
#git push -u origin master
git push
