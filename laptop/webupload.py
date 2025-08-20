# -*- coding: utf-8 -*-
import paramiko
import os

import requests


def Data_upload(group,phenotype,gps):
    #phenotype = '1242.22,52532,12.22,253.2'
    url = 'http://110.40.172.86:8080/Fuwuqi_war/SetPhenotype?username=hzau&group='+str(group)+\
          '&phenotype=' + phenotype +'&gps=' + gps
    # url = 'http://localhost:8080/SetPhenotype?username=hzau&group=8&phenotype='+phenotype

    result = requests.get(url).content
    print(result)
def Ftp_upload(filename, target_path="/pic/"):
    pkey = './Fuwuqi_6'
    key = paramiko.RSAKey.from_private_key_file(pkey, password='******')
    #paramiko.util.log_to_file('paramiko.log')

    ssh = paramiko.SSHClient()
    ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    ssh.connect('110.40.172.86', username='root', password=', pkey=key)
    t = ssh.get_transport()
    sftp = paramiko.SFTPClient.from_transport(t)
    d = sftp.put(filename, target_path+filename)
    t.close()

def upload(group,phenotype,gps):
    Data_upload(group, phenotype, gps)
    Ftp_upload(str(group)+'.jpg', target_path="/pic/")
    print('done!')

if __name__ == '__main__':
    upload(1,'12.124,2.123,25.324,22.114','12.33,22.22')