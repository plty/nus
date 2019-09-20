while read p; do
    openssl enc -d -aes-128-ecb -nosalt -base64 -pass pass:$p -in flag.txt.enc | grep flag
    back=`echo -n $p | tail -c 3`
    if [ $back == "aaa" ]; then
        echo "$p"
    fi
done
