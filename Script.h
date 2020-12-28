//
// Created by dogaucak on 12/24/2020.
//

#ifndef FTP_SERVER_SCRIPT_H
#define FTP_SERVER_SCRIPT_H

#define PASS_CHECK_SCRIPT "\
#!/bin/bash \n\
USERNAME=\"chng_val_username\" \n\
export PASSWD=\"chng_val_password\" \n\
ORIGPASS=`grep -w \"$USERNAME\" /etc/shadow | cut -d: -f2` \n\
export ALGO=`echo $ORIGPASS | cut -d\"$\" -f2` \n\
export SALT=`echo $ORIGPASS | cut -d\"$\" -f3` \n\
GENPASS=\"$(perl -e 'print crypt(\"$ENV{PASSWD}\",\"\\$$ENV{ALGO}\\$$ENV{SALT}\\$\")')\" \n\
if [ \"$GENPASS\" = \"$ORIGPASS\" ]; then \n\
    exit 0 \n\
else \n\
    exit 1 \n\
fi \n\
"

#endif //FTP_SERVER_SCRIPT_H
