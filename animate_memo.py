import io

import matplotlib.pyplot as plt

def __read_state(file):
    memo = []
    
    line = file.readline().strip("\r\n")
    if line == "":
        return (None, None, None)

    m, n = map(int, line.split(" "))

    line = file.readline().strip("\r\n")
    while line != "":
        memo.append(line.split(";")[:-1])
        line = file.readline().strip("\r\n")

    return m - 1, n - 1, memo

def __create_table(a, b, memo):
    table = plt.table(
        cellText=memo,
        rowLabels=["  %s  " % (x) for x in a],
        colLabels=list(b),
        loc='center',
        cellLoc='center'
    )
    table.auto_set_font_size(False)

    cells = table.get_celld()

    table_cells=table.get_children()
    for cell in table_cells:
        cell.set_width(1.0 / (len(b) + 1))
        cell.set_height(1.0 / (len(a) + 1))

        cell.set_fontsize(16.0)
        cell.auto_set_font_size(cell.figure.canvas.get_renderer())

    for c, cell in cells.items():
        if c[0] > 0 and c[1] >= 0:
            cell.set_fill(True)

    return table, cells

def __compute_accesses(a, b, m, n, accesses):
    if a[m] == b[n]:
        if m > 0 and n > 0:
            accesses[m - 1][n - 1] += 1
    else:
        for c in [(m - 1, n), (m, n - 1)]:
            if c[0] >= 0 and c[1] >= 0:
                accesses[c[0]][c[1]] += 1

if __name__ == "__main__":
    
    file = open("memo_log", 'r')
    a = file.readline().strip("\r\n")
    b = file.readline().strip("\r\n")

    # Configuração do PyPlot
    plt.ion()
    plt.axis('off')
    plt.rcParams["font.family"] = "serif"

    plt.waitforbuttonpress()
    plt.waitforbuttonpress()

    # Variáveis de controle
    table = None
    caption = None

    memo = None

    step = 0

    # Tabela de acessos por célula da matriz
    accesses = []
    for _ in range(len(a)):
        accesses.append([0] * len(b))

    # Função de normalização para a matriz: Cada célula é usada
    # no mínimo 1 vezes e no máximo 3
    normal = plt.Normalize(0, 4)

    while True:
        step += 1

        m, n, state = __read_state(file)

        if state is None:
            break

        memo = state

        __compute_accesses(a, b, m, n, accesses)

        if table is None:
            table, cells = __create_table(a, b, memo)

        current_cell = cells[m + 1, n]

        original_face_color = current_cell.get_facecolor()
        original_text_color = current_cell.get_text().get_color()

        for i, j in [(m, n)] + ([(m - 1, n - 1)] if a[m] == b[n] else [(m - 1, n), (m, n - 1)]):
            if i < 0 or j < 0:
                continue

            cell = cells[i + 1, j]

            cell.get_text().set_text(memo[i][j])
            
            cell.set_fontsize(16.0)
            cell.auto_set_font_size(cell.figure.canvas.get_renderer())

            if cell == current_cell:
                cell.set_fc("red")
            else:
                color = plt.cm.viridis(normal(accesses[i][j]))
                cell.set_fc(color)

            cell.get_text().set_color("white")

        if caption is None:
            caption = plt.text(0, 1.1, "", fontsize=14)

        caption.set_text('Step: %d      m: %d       n: %d' % (step, m, n))
        
        plt.draw()
        plt.pause(0.001)

        current_cell.set_fc(original_face_color)
        current_cell.get_text().set_color(original_text_color)

    lcs = ""

    m = len(a) - 1
    n = len(b) - 1

    while m >= 0 and n >= 0:
        cell = cells[m + 1, n]
        cell.set_fc("red")
        cell.get_text().set_color("white")

        if a[m] == b[n]:
            lcs = a[m] + lcs
            m -= 1
            n -= 1

        elif memo[m - 1][n] >= memo[m][n - 1]:
            m -= 1

        else:
            n -= 1

        cell.get_text().set_text(lcs)
        cell.auto_set_font_size(cell.figure.canvas.get_renderer())

        plt.draw()
        plt.pause(0.1)

    plt.ioff()
    caption = plt.text(
        0, -0.1, 
        "Finished: LCS is \"%s\", %d memo accesses" % (
            lcs,
            sum([item for sub in accesses for item in sub])), 
            fontsize=14
        )

    plt.show()