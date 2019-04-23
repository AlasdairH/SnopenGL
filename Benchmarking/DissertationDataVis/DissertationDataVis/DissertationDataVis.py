import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.cbook as cbook
import pandas as pd
import math
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

    # the per frame values for time taken to execute
    data_frame          = defaultdict(list)
    data_shadowMap      = defaultdict(list)
    data_ColDetTFB      = defaultdict(list)
    data_visuals        = defaultdict(list)
    data_particleSim    = defaultdict(list)

    # the per frame percentages of the total each operation took
    data_avgShadowMap      = defaultdict(list)
    data_avgColDetTFB      = defaultdict(list)
    data_avgVisuals        = defaultdict(list)
    data_avgParticleSim    = defaultdict(list)
    rawAvgData = { }

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

                    total = data_shadowMap[_files[i]][-1] + data_ColDetTFB[_files[i]][-1] + data_visuals[_files[i]][-1] + data_particleSim[_files[i]][-1]

                    data_avgShadowMap[_files[i]].append(data_shadowMap[_files[i]][-1] / total)
                    data_avgColDetTFB[_files[i]].append(data_ColDetTFB[_files[i]][-1] / total)
                    data_avgVisuals[_files[i]].append(data_visuals[_files[i]][-1] / total)
                    data_avgParticleSim[_files[i]].append(data_particleSim[_files[i]][-1] / total)
             

                lineCount += 1
    rawAvgData = { "orangeBars": data_avgShadowMap[_files[0]], "blueBars": data_avgColDetTFB[_files[0]], "greenBars": data_avgVisuals[_files[0]], "redBars": data_avgParticleSim[_files[0]] }
    df = pd.DataFrame(rawAvgData)

    print("Parsed " + str(lineCount) + " records")

    figure1 = plt.figure(figsize = (16, 9), dpi = 100)
    renderPlot = 4
    for i in range(renderPlot, renderPlot + 1):
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
           
            
            plot = axes.plot(data_frame[_files[0]], data_particleSim[_files[0]], colours[j], label = _files[j]);
            #plot = axes.plot(np.arange(1000, 1000 + len(avg), 1).tolist(), avg, "m");

        if(i == 6):
            graphRange = 1
            axes.set_title("Percentage Usage")
            axes.bar(data_frame[_files[0]][0:graphRange], data_avgShadowMap[_files[0]][0:graphRange], color='red', edgecolor='white', width = 1, label = "Shadow Mapping")
            axes.bar(data_frame[_files[0]][0:graphRange], data_avgColDetTFB[_files[0]][0:graphRange], bottom = data_avgShadowMap[_files[0]][0:graphRange], color='green', edgecolor='white', width = 1, label = "WS Geometry Feedback")
            axes.bar(data_frame[_files[0]][0:graphRange], data_avgVisuals[_files[0]][0:graphRange], bottom = data_avgColDetTFB[_files[0]][0:graphRange], color='blue', edgecolor='white', width = 1, label = "Visuals")
            axes.bar(data_frame[_files[0]][0:graphRange], data_avgParticleSim[_files[0]][0:graphRange], bottom = data_avgVisuals[_files[0]][0:graphRange], color='orange', edgecolor='white', width = 1, label = "Particle Simulation")

            axes.set_ylabel("Percentage of total frame time")

        startX, endX = axes.get_xlim()
        startY, endY = axes.get_ylim()

        axes.legend()

        axes.xaxis.set_ticks(np.arange(math.floor(startX), math.ceil(endX) + 1, 100))
        axes.yaxis.set_ticks(np.arange(startY, endY, 0.5))

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