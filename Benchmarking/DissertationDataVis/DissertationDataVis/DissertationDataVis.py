import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.cbook as cbook
from collections import defaultdict
import numpy as np
import csv
import sys
import os

def running_mean(x, N):
    cumsum = np.cumsum(np.insert(x, 0, 0))
    return (cumsum[N:] - cumsum[:-N]) / float(N)

def parseData(_files):
    print("Starting data read")

    colours = { 0: "b", 1: "r", 2: "g", 3: "c", 4: "m", 5: "k", 6: "y" }

    data_frame          = defaultdict(list)
    data_shadowMap      = defaultdict(list)
    data_ColDetTFB      = defaultdict(list)
    data_visuals        = defaultdict(list)
    data_particleSim    = defaultdict(list)

    for i in range(0, len(_files)):
        with open(_files[i]) as csvFile:
            csvReader = csv.reader(csvFile, delimiter = ',')
            lineCount = 0
            for row in csvReader:
                if(lineCount == 0):
                    print("Read Headings")
                else:
                    data_frame[_files[i]].append(int(row[0]))
                    data_shadowMap[_files[i]].append(int(row[1]) / 1000000)
                    data_ColDetTFB[_files[i]].append(int(row[2]) / 1000000)
                    data_visuals[_files[i]].append(int(row[3]) / 1000000)
                    data_particleSim[_files[i]].append(int(row[4]) / 1000000)

                lineCount += 1

    print("Parsed " + str(lineCount) + " records")

    figure1 = plt.figure(figsize = (16, 9), dpi = 100)
    for i in range(4, 5):
        axes = figure1.add_subplot(1, 1, 1)
        axes.set_xlabel("Frame")
        axes.set_ylabel("Time (milliseconds )")

        if(i == 1):
            axes.set_title("Shadow Mapping")
            for j in range(0, len(_files)):
                plot = axes.plot(data_frame[_files[j]], data_shadowMap[_files[j]], colours[j], label = _files[j]);

        if(i == 2):
            axes.set_title("World Space Geometry Transform Feedback")
            for j in range(0, len(_files)):
                plot = axes.plot(data_frame[_files[j]], data_ColDetTFB[_files[j]], colours[j], label = _files[j]);

        if(i == 3):
            axes.set_title("Visual Rendering")
            for j in range(0, len(_files)):
                plot = axes.plot(data_frame[_files[j]], data_visuals[_files[j]], colours[j], label = _files[j]);

        if(i == 4):
            axes.set_title("Particle Simulation")
            for j in range(0, len(_files)):
                avg = running_mean(data_particleSim[_files[j]], 50).tolist()

                plot = axes.plot(data_frame[_files[j]], data_particleSim[_files[j]], colours[j], label = _files[j]);
                #plot = axes.plot(np.arange(1000, 1000 + len(avg), 1).tolist(), avg, "m");
        if(i == 5):
            axes.set_title("Particle Collision Detection")
            
            avg = running_mean(data_particleSim[_files[j]], 50).tolist()
            
            plot = axes.plot(data_frame[_files[j]], data_particleSim[_files[j]], colours[j], label = _files[j]);
            #plot = axes.plot(np.arange(1000, 1000 + len(avg), 1).tolist(), avg, "m");

        startX, endX = axes.get_xlim()
        startY, endY = axes.get_ylim()

        axes.legend()

        axes.xaxis.set_ticks([1000, 2000, 3000])
        axes.yaxis.set_ticks(np.arange(startY, endY + 1, 1.0))

    figure1.subplots_adjust(hspace = 0.8)
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