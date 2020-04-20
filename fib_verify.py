#!/usr/bin/env python3
import re
import os
import time
import pathlib
import argparse
import subprocess
import requests
import colorama
import urllib.request
from colorama import Fore, Style

def fetch_fib_number(index):
    resp = urllib.request.urlopen(f"http://www.protocol5.com/Fibonacci/{index}.htm")
    html = [h.strip().decode('utf-8') for h in resp.readlines()]
    represents = []
    for line in html:
        match = re.search(r"<li><h4>.+?</h4><div>(.+?)</div></li>", line)
        if match:
            represents.append(match.group(1))

    return {
        "base2": represents[0],
        "base3": represents[1],
        "base5": represents[2],
        "base6": represents[3],
        "base8": represents[4],
        "base10": represents[5],
        "radix16": represents[6],
        "radix36": represents[7],
        "radix63404": represents[8]
    }

def main(url, index, base):
    start_time = time.time()
    res = requests.get(url)
    if res.status_code == requests.codes.ok :
        duration = time.time() - start_time
        print(f"fibonacci({index}) = {res.text}")
        print(f"time cost {duration} (s)")
        expect = fetch_fib_number(args.index)[base]
        if res.text == expect:
            print(Fore.GREEN + "-----------------------Pass----------------------------")
        else:
            print(Fore.RED + "------------------------No pass--------------------------")
        
        exit(0)
    else:
        print(Fore.RED + "-----------------------Fail-----------------------")
        exit(1)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("port", type=str, help="Fibonacci URL PORT")
    parser.add_argument("index", type=str, help="Fibonacci number index")
    parser.add_argument("-b", action='store', dest="base", help="Fibonacci number base (default: base10)")
    args = parser.parse_args()
    
    FIB_URL = "http://localhost:" + args.port + "/fib/" + args.index
    FIB_INDEX = int(args.index)
    FIB_NUMBER_BASE = args.base

    if FIB_INDEX < 0:
        print("Fibonacci index is less than 0")
        exit(1)

    if not FIB_NUMBER_BASE:
        FIB_NUMBER_BASE = "base10"

    main(FIB_URL, FIB_INDEX, FIB_NUMBER_BASE)
