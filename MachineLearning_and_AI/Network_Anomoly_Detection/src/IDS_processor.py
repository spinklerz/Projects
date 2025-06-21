import pandas as pd


def getData(n_seconds): 
    ''' Function to get data from zeek every n seconds'''
    # File path = /opt/homebrew/Cellar/zeek/7.1.0/logs/current
    file_object = open("/opt/homebrew/Cellar/zeek/7.1.0/logs/current/conn.log")
    all_lines = file_object.readlines()
    position = 0
    with file_object as f: 
        position = f.tell()

    pass
def main(): 
    n_seconds = 60
    getData(n_seconds)
    pass

main()