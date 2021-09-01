#!/usr/bin/env python
import sys
import argparse

##################################################################
# Main
##################################################################
def main(argv):
    
    parser = argparse.ArgumentParser()
    parser.add_argument('-t', dest='type',   default='uart',                  help='Device type (uart|ftdi)')
    parser.add_argument('-d', dest='device', default='/dev/ttyUSB1',          help='Serial Device')
    parser.add_argument('-b', dest='baud',   default=1000000,       type=int, help='Baud rate')
    parser.add_argument('-a', dest='address',required=True,                   help='Address to write')
    parser.add_argument('-v', dest='value',  required=True,                   help='Value to write')
    args = parser.parse_args()

    addr   = int(args.address, 0)
    value  = int(args.value, 0)

    print("The test value:", addr, value)

if __name__ == "__main__":
   main(sys.argv[1:])
