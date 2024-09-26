#!/bin/bash
set -x

find . -name "*.out"  | xargs rm -f
find . -name "core.*"  | xargs rm -f

user_name="dddw1216"
user_email="dddw1216@gmail.com"

git config user.name ${user_name}
git config user.email ${user_email}

MESSAGE=`date +"%Y-%m-%d %T"`

git add --all .
git commit -m "${MESSAGE}"
git push origin main
