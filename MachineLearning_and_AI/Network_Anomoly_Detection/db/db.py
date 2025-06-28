import sqlite3
import os 

R = "\033[91m"  # Red text
W = "\033[0m"   # Reset to default
G = "\033[92m"  # Green text
'''
CREATE TABLE table_name (
    column_name data_type constraints,
    column_name data_type constraints,
    ...
);
'''
# Init db 
DATABASE = "anomalous_db.db"

def init_db():
    conn = sqlite3.connect(DATABASE)
    cursor = conn.cursor() 
    
    # Simple table for now
    cursor.execute('''CREATE TABLE IF NOT EXISTS anomalous(
        src_ip TEXT NOT NULL,
        dst_ip TEXT NOT NULL, 
        sport INTEGER, 
        dport INTEGER, 
        proto TEXT NOT NULL
    )''')
    conn.commit()
    conn.close()
    print(f"{G}Database initialized successfully🥳{W}")

def sample_data():
    conn = sqlite3.connect(DATABASE)
    cursor = conn.cursor() 
    cursor.execute("INSERT INTO anomalous(src_ip,dst_ip,sport,dport,proto) VALUES (?, ?, ?, ?, ?)", ("192.168.1.25", "192.168.1.1", 35728, 21, "ftp"))
    conn.commit()
    conn.close()
    print(f'{G}Sample Data Added!😡{W}')
    pass

def dump_all_activity():
    conn = sqlite3.connect(DATABASE)
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM anomalous")
    data = cursor.fetchall()
    print(f"\n{G}*****ALL THE FALSE POSITIVES LETS GOO******{W}")
    print(f"{'SRC IP': <15} {'DST IP': <15} {'SRC PORT': <15} {'DST PORT': <15} {'PROTO': <5}")
    for x in data:
        print(f"{x[0]: <15} {x[1]: <15} {x[2]: <15} {x[3]: <15} {x[4]: <5}")
    pass
def main():
    init_db()
    sample_data()
    dump_all_activity()

# Fully functional v1.0 