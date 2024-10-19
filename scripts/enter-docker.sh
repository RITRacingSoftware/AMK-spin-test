#!/usr/bin/env bash

docker run -it \
	--mount type=bind,source=$PWD,destination=/spin_test \
	spin_test
