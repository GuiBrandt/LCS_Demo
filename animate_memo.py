import io
import matplotlib.pyplot as plt

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

    normal = plt.Normalize(-1, 4)

    while True:
        step += 1
        
        line = file.readline()
        if line == "":
            break

        cell_text = []

        m, n = map(int, line.split(" "))

        if a[m - 1] == b[n - 1]:
            if m > 0 and n > 0:
                accesses[m - 1][n - 1] += 1

        else:
            for tp in [(m - 1, n), (m, n - 1)]:
                if tp[0] >= 0 and tp[1] >= 0:
                    accesses[tp[0]][tp[1]] += 1

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
                    cell.set_fill(True)

        colors = plt.cm.viridis(normal(accesses))
        cells = table.get_celld()
        current_cell = cells[m + 1, n]

        last_fc = current_cell.get_fc()
        last_tc = current_cell.get_text().get_color()

        for c in [(m, n), (m - 1, n - 1), (m - 1, n), (m, n - 1)]:
            i, j = c

            if i < 0 or j < 0:
                continue

            cell = cells[i + 1, j]
            cell.get_text().set_text(cell_text[i][j])

            if cell == current_cell:
                cell.set_fc("red")
                cell.get_text().set_color("white")
            else:
                cell.set_fc(colors[i][j])
                cell.get_text().set_color("white")

        if caption is None:
            caption = plt.text(0, 1.1, "", fontsize=16)

        caption.set_text('Step: %d      m: %d       n: %d' % (step, m, n))
        
        plt.draw()
        plt.pause(0.01)

        current_cell.set_fc(last_fc)
        current_cell.get_text().set_color(last_tc)

    plt.ioff()
    caption = plt.text(
        0, -0.1, 
        "Finished: LCS is \"%s\", %d memo accesses" % (
            cell_text[len(a) - 1][len(b) - 1],
            sum([item for sub in accesses for item in sub])), 
            fontsize=16
        )

    plt.show()