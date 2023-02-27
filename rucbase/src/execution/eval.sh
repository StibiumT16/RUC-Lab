rm output.txt
cat eval.sql | while read line
do
    if [ ${#line} -eq 0 ] || [ ${line:0:1} == "#" ]
    then
        echo "$line"
        continue
    fi
    echo ">> $line"
    ../../build/bin/exec_sql "$line"
    echo "------------------------------"
done | tee -a output.txt