import io
import matplotlib.pyplot as plt

from time import sleep

if __name__ == "__main__":
    
    file = open("memo_log", 'r')
    a = " " + file.readline().strip("\r\n")
    b = " " + file.readline().strip("\r\n")

    plt.ion()
    table = None

    while True:
        cell_text = []
        
        line = file.readline()
        if line == "":
            break

        line = line.strip("\r\n")
        while line != "":
            cell_text.append(line.split(";")[:-1])
            line = file.readline().strip("\r\n")

        if table is None:
            table = plt.table(
                cellText=cell_text,
                rowLabels=list(a),
                colLabels=list(b),
                loc='center'
            )
            table.set_fontsize(18)
            table.scale(1, 2)

        else:
            for i in range(len(cell_text)):
                for j in range(len(cell_text[i])):
                    table.get_celld()[(i + 1, j)].get_text().set_text(cell_text[i][j])

        plt.axis('off')
        plt.draw()

        plt.pause(0.01)

    plt.pause(5)