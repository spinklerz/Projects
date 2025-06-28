import json
import requests
import argparse

def parse_arguments():
    '''
    parse arguements for v1 it will only allow an option for a domain or list of domains
    '''
    parser = argparse.ArgumentParser(description="Crt.sh subdomain finder - A tool using a certificate transparency log search engine to scan for subdomains.\n Only use on authorized targets!\nExample: python3 crt.py -d example.com -o output.txt")
    parser.add_argument('-d', '--domain', help='Target Domain')
    parser.add_argument('-o', '--output-file', help='Outputfile', required=True)
    return parser.parse_args()
    
def main():
    '''
    This Function will query crt.sh and parse it for subdomains
    Key Notes:
        - Simple Implementation for single queries
    '''
    args = parse_arguments()
    
    # Arg check
    if not(args.domain or args.domain_list):
        print("Options -d or -dL are mandatory!")
        return 1
    if args.domain: 
        r = requests.get(f'https://crt.sh/json?q={args.domain}')
        data = r.json()
        # print(data)
        domain_list = set()
        for item in data:
            domain_list.add(item['common_name'])
            if '\n' in item['name_value']:
                temp = list(item['name_value'].split('\n'))
                for x in temp: 
                    domain_list.add(x)
            else:
                domain_list.add(item['name_value'])
        with open(args.output_file, 'w') as f:
            for x in domain_list:
                f.write(f'{x}\n')
            
    
main()
