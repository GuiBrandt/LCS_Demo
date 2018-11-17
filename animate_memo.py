import io
import matplotlib.pyplot as plt
import numpy as np

from time import sleep

if __name__ == "__main__":
    
    file = open("memo_log", 'r')
    a = " " + file.readline().strip("\r\n")
    b = " " + file.readline().strip("\r\n")

    plt.ion()
    plt.axis('off')
    plt.rcParams["font.family"] = "serif"

    table = None
    caption = None
    step = 0

    accesses = []
    for _ in range(len(a)):
        accesses.append([0] * len(b))
    max_accesses = 0

    while True:
        step += 1
        cell_text = []
        
        line = file.readline()
        if line == "":
            break

        m, n = map(int, line.split(" "))

        if a[m - 1] == b[n - 1]:
            if m > 0 and n > 0:
                accesses[m - 1][n - 1] += 1

            acc = accesses[m - 1][n - 1]
            max_accesses = max_accesses if max_accesses > acc else acc

        else:
            for tp in [(m - 1, n), (m, n - 1)]:
                if tp[0] >= 0 and tp[1] >= 0:
                    accesses[tp[0]][tp[1]] += 1

                    acc = accesses[tp[0]][tp[1]]
                    max_accesses = max_accesses if max_accesses > acc else acc

        line = file.readline().strip("\r\n")
        while line != "":
            cell_text.append(line.split(";")[:-1])
            line = file.readline().strip("\r\n")

        if table is None:
            table = plt.table(
                cellText=cell_text,
                rowLabels=["  %s  " % (x) for x in a],
                colLabels=list(b),
                loc='center',
                cellLoc='center'
            )

            table_cells=table.get_children()
            for cell in table_cells:
                cell.set_width(1.0 / (len(b) + 1))
                cell.set_height(1.0 / (len(a) + 1))
                cell.set_fontsize(14)

            for c, cell in table.get_celld().items():
                if c[0] > 0 and c[1] >= 0:
                    cell.get_text().set_color("white")
                    cell.set_fill(True)                    

        else:
            normal = plt.Normalize(-1, max_accesses + 1)
            colors = plt.cm.viridis(normal(accesses))

            for c, cell in table.get_celld().items():
                i, j = c

                if i > 0 and j >= 0:
                    cell.get_text().set_text(cell_text[i - 1][j])

                    if i - 1 == m and j == n:
                        cell.set_fc("red")
                    else:
                        cell.set_fc(colors[i - 1][j])

        if caption is None:
            caption = plt.text(0, 1.1, "", fontsize=18)

        caption.set_text('Step: %d' % (step))
        
        plt.draw()
        plt.pause(0.01)

    plt.ioff()
    plt.show()