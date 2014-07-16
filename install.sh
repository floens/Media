ps -W | awk '/DMedia.exe/,NF=1' | xargs kill -f
cp output.exe "$HOMEDRIVE/Program Files (x86)/ASUS/ATK Package/ATK Media/DMedia.exe"
"$HOMEDRIVE/Program Files (x86)/ASUS/ATK Package/ATK Media/DMedia.exe" &
