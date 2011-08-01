#!/bin/bash

if [ -n "$1" ]; then
	git config remote.origin.url "git@github.com:librae8226/stm32_dev_linux.git"
	echo ''
	echo "Start synchronizing..."
	echo ''
	if [ $1 == "push" ]; then
		git add .
		git commit -a
		echo "pushing..."
		git push origin master
		git push --tags
	elif [ $1 == "pull" ]; then
		echo "pulling..."
		git pull origin master
	else
		echo "error arg."
		echo "USAGE: sync push|pull"
		exit
	fi
else
	echo "USAGE: sync push|pull"
	exit
fi

echo ''
echo 'Synchronize ok.'
echo ''

