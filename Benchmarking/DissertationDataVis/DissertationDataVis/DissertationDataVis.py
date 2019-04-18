import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.cbook as cbook
import numpy as np
import csv
import sys

def main():
    print("Starting data read")

    data_frame = []
    data_shadowMap = []
    data_ColDetTFB = []
    data_visuals = []
    data_particleSim = []

    with open("data.csv") as csvFile:
        csvReader = csv.reader(csvFile, delimiter = ',')
        lineCount = 0
        for row in csvReader:
            if(lineCount == 0):
                print("Read Headings")
            else:
                data_frame.append(int(row[0]))
                data_shadowMap.append(int(row[1]))
                data_ColDetTFB.append(int(row[2]))
                data_visuals.append(int(row[3]))
                data_particleSim.append(int(row[4]))

            lineCount += 1

    print("Parsed " + str(lineCount) + " records")

    figure1 = plt.figure(figsize = (16, 9), dpi = 100)
    axes = figure1.add_subplot(1, 1, 1)
    axes.set_title("Frames per Second")
    plt.xlabel("Frame")
    plt.ylabel("Time (Nanoseconds)")

    axes.plot(data_frame, data_shadowMap, "b");
    axes.plot(data_frame, data_ColDetTFB, "r");
    axes.plot(data_frame, data_visuals, "g");
   # axes.plot(data_frame, data_particleSim, "k");

    startX, endX = axes.get_xlim()
    startY, endY = axes.get_ylim()

    axes.xaxis.set_ticks([1000, 2000])
    axes.yaxis.set_ticks([startY, endY])

    plt.show()

main()