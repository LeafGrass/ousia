#!/bin/bash

if [ -n "$1" ]; then
	git config remote.origin.url "git@github.com:librae8226/ousia.git"
	echo ''
	echo "Start synchronizing..."
	echo ''
	if [ $1 == "push" ]; then
		git add .
		git commit -a
		echo "pushing..."
		git push --all
		git push --tags
	elif [ $1 == "pull" ]; then
		echo "pulling..."
		git pull --all
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

