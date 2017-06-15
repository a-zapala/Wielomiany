#!/bin/sh
if [ $# -eq 2 ]  ; then #ilosc argumentow rowna 2
    
    isExit=0
    
    if ! [ -d $2 ] ; then
    echo "Nieprawidlowa sciezka"
    isExit=1
    fi;
    
    if ! [ -x $1 ] ; then
    echo "Nieprawidlowy plik"
    isExit=1
    fi;
    
    if [ ${isExit} -eq 1 ] ; then
    exit 1
    fi;
        
    for f in $2/*; do 
    
        pom=$(head -1 "$f")
        
        if [ "$pom" = "START" ] ; then
            target=$f
        fi;
    done
    
    isStop=0
    
    touch pom1.in
    touch pom2.in
    
    while [ $isStop -eq 0 ]; do
       
        while read line           
        do     
        
        if [ "$line" = "STOP" ]; then
        cat pom2.in pom1.in  > pom3.in 2>/dev/null
        rm pom2.in
        $1 < pom3.in 
        isStop=1
        
        else
            if [ "${line%%" "*}" = "FILE" ]; then
                target="$2/${line#*" "}"
                cat pom2.in pom1.in  > pom3.in 
                rm pom2.in
                $1 < pom3.in > pom2.in 2>/dev/null
                rm pom3.in
                rm pom1.in
                touch pom1.in
            else
                if [ "$line" = "START" ]; then
                    isStop=0;
                else
                echo $line >> pom1.in
                fi;
            fi;
        fi;       
        done <$target
        
   done
   rm -f pom1.in pom2.in pom3.in
else
    echo "Zla liczba argumentow"
    exit 1
fi;