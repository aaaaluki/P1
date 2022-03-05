#!/usr/bin/env python

import enum
import sys
from math import cos, pi, sin
from typing import List, Tuple

import matplotlib.pyplot as plt
import numpy as np


# Plot all lines in the same plot or not
ONE_FOR_ALL = False

# Constants for generating the sample file
FSINE = 1000
FCOSINE = 100
FS = 4*max(FSINE, FCOSINE)
NSAMPLES = 1024


class ArgType(enum.Enum):
    NULL = 0
    GEN_SAMPLE = 1
    PLOT = 2


# Formated info message
def info(msg: str) -> None:
    print('\033[0;32m[INFO]\033[0m {}'.format(msg))


# Formated error message and exit with error
def error(msg: str, print_usage: bool =False) -> None:
    print('\033[1;91m[ERROR]\033[0m {}'.format(msg))
    if print_usage:
        usage()

    sys.exit(1)


# Prints explains how to use the script, kinda...
def usage() -> None:
    print('Usage: python {} {{generate|plot}} [csv_file]'.format(sys.argv[0]))


# Parse given arguments and set settings (command, filename)
def setup(args: List[str]) -> Tuple[ArgType, str]:
    if len(args) < 2:
        error('No argument was given!', True)

    if args[1] == 'generate':
        if len(args) < 3:
            return (ArgType.GEN_SAMPLE, None)
        else:
            return (ArgType.GEN_SAMPLE, args[2])

    elif args[1] == 'plot':
        if len(args) < 3:
            error('CSV file needed for plotting!')

        return (ArgType.PLOT, args[2])

    elif args[1] == 'help':
        usage()
        return (ArgType.NULL, None)

    else:
        error('The given argument is not valid!', True)


# Generate a sample .csv file
def gen_sample_file(filename: str) -> None:
    if filename is None:
        filename = 'sample-sines.csv'

    with open(filename, 'w') as file:
        # Write tags
        file.write('t,sine,cosine\n')

        # Write data
        for i in range(NSAMPLES):
            line = '{:.4f}'.format(i / FS)
            line += ',{:.4f}'.format(sin(2*pi*(FSINE/FS)*i))
            line += ',{:.4f}'.format(cos(2*pi*(FCOSINE/FS)*i))

            line += '\n'

            file.write(line)

    info('Sample data saved in {}'.format(filename))


# Plot data from csv file
def plotter(filename: str) -> None:
    if filename.endswith('.csv'):
        separator_char = ','
    else:
        # Let's suppose it's a TSV file
        separator_char = '\t'

    info('Plotting data from {}'.format(filename))

    with open(filename, 'r') as file:
        # Get data
        # The data will be organized in columns on a numpy array, each column
        # representing a column in the csv file
        initialized = False
        for l in file.readlines():
            nums = [float(n) for n in l.strip().split(separator_char)]

            if not initialized:
                data = np.array(nums)
                initialized = True
                continue
            
            data = np.vstack((data, np.array(nums)))

    if not initialized:
        error('The file {} has no data!'.format(filename))
    
    titles = ['x'] + ['col{}'.format(i) for i in range(1, data.shape[1])]

    # Plot data
    if ONE_FOR_ALL:
        plt.title('Plot for file {}'.format(filename))
        for i in range(1, len(titles)):
            plt.plot(data[:, 0], data[:, i])
        
            plt.xlim((data[:, 0][0], data[:, 0][-1]))

            plt.xlabel(titles[0])
            plt.legend(titles[1::])
            plt.grid(which='both', color='#777777', linestyle=':', linewidth=0.5)

    else:
        fig, axs = plt.subplots(len(titles) - 1, 1)
        fig.suptitle('Plot for file {}'.format(filename))
        for i in range(1, len(titles)):
            axs[i - 1].plot(data[:, 0], data[:, i])
        
            axs[i - 1].set_xlim((data[:, 0][0], data[:, 0][-1]))

            axs[i - 1].set_xlabel(titles[0])
            axs[i - 1].set_ylabel(titles[i])
            axs[i - 1].grid(which='both', color='#777777', linestyle=':', linewidth=0.5)

        fig.tight_layout()

    # Save plot to PNG file
    idx = filename.rfind('.')
    savefile ='{}.png'.format(filename[:idx])
    plt.savefig(savefile, dpi=300)
    info('Plot saved as {}'.format(savefile))

    # Display plot
    plt.show()


# Main
def main(args: Tuple[ArgType, str]) -> None:
    if args[0] == ArgType.GEN_SAMPLE:
        gen_sample_file(args[1])
    elif args[0] == ArgType.PLOT:
        plotter(args[1])


if __name__ == '__main__':
    main(setup(sys.argv))
    sys.exit(0)
