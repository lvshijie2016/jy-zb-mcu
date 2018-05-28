#!/bin/sh

APP_FILE=app.bin
IAP_FILE=$1
APP_FILE=$2
OUT_FILE=$3

# app.bin size 1M-124K
tr '\000' '\377' < /dev/zero| dd of=_iap.bin bs=6 count=1024
dd conv=notrunc if=$IAP_FILE of=_iap.bin

cat _iap.bin $APP_FILE > $OUT_FILE
rm _iap.bin
