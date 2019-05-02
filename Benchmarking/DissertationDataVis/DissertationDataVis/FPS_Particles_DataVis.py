import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.cbook as cbook
from collections import defaultdict
import numpy as np
import math
import csv
import sys
import os

def running_mean(x, N):
    cumsum = np.cumsum(np.insert(x, 0, 0))
    return (cumsum[N:] - cumsum[:-N]) / float(N)

def parseData(_files):
    print("Starting data read")

    colours = { 0: "b", 1: "r", 2: "g", 3: "c", 4: "m", 5: "k", 6: "y" }

    data_particleCount  = defaultdict(list)
    data_simTime        = defaultdict(list)
    data_FPS            = defaultdict(list)

    for i in range(0, len(_files)):
        with open(_files[i]) as csvFile:
            csvReader = csv.reader(csvFile, delimiter = ',')
            lineCount = 0
            for row in csvReader:
                if(lineCount == 0):
                    print("Read Headings")
                else:
                    data_particleCount[_files[i]].append(int(row[0]))
                    data_simTime[_files[i]].append(float(row[1]))
                    data_FPS[_files[i]].append(float(row[2]))
                lineCount += 1


    figure1 = plt.figure(figsize = (16, 9), dpi = 100)

    axes = figure1.add_subplot(1, 1, 1)
    axes.set_xlabel("Particle Count")
    axes.set_ylabel("Frames per Second")

    axes.set_title("Particle Count vs Frames per Second")

    #plot = axes.plot(data_particleCount[_files[0]], data_simTime[_files[0]], "b", label = "Particle Processing Time");
    #plot = axes.plot(data_particleCount[_files[0]], data_simTime[_files[0]], "r", label = "Milliseconds", zorder = 1);
    plot = axes.plot(data_particleCount[_files[0]], data_FPS[_files[0]], "r", label = "Frames per Second", zorder = 1);

    startX, endX = axes.get_xlim()
    startY, endY = axes.get_ylim()

    axes.legend()

    axes.xaxis.set_ticks(np.arange(0, 1100000, 100000.0))
    axes.yaxis.set_ticks(np.arange(0, 800, 100.0))

    axes.axhline(y = 60, label = "60FPS", linestyle = "--", zorder = 0)

    #figure1.subplots_adjust(hspace = 0.8)
    plt.show()

def getFileNames():
    files = []
    for file in os.listdir("./"):
        if file.endswith(".csv"):
            files.append(os.path.join("", file))

    print("Found " + str(len(files)) + " CSV file(s)")
    return files

files = getFileNames()
parseData(files)
