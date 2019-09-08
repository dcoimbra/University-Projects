import copy
import enum


class Celula(enum.Enum):
    VAZIO = 0
    BRANCO = 1
    CAIXA = 2


class Coordenada():
    def __init__(self, linha, coluna):
        if not(isinstance(linha, int) and isinstance(coluna, int) and
                linha >= 1 and coluna >= 1):
            raise ValueError('Coordenada: argumentos invalidos')

        self.coordenada = (linha, coluna)

    def linha(self):
        return self.coordenada[0]

    def coluna(self):
        return self.coordenada[1]

    def __eq__(self, outra):
        if not isinstance(outra, Coordenada):
            raise ValueError('Coordenada =: argumentos invalidos')

        return self.linha() == outra.linha() and \
            self.coluna() == outra.coluna()

    def __str__(self):
        return f"({self.linha()} : {self.coluna()})"


class Tabuleiro():
    def __init__(self, t):
        if not verifica_conf(t):
            raise ValueError("Tabuleiro: argumentos invalidos")

        self.dimensao = len(t[0])
        self.conf = t
        self.tabuleiro = [[Celula.VAZIO for x in t[1]] for x in t[0]]

    def dimensoes(self):
        return (self.dimensao, self.dimensao)

    def celula(self, c):
        if not (isinstance(c, Coordenada) and
                self.coordenada_compativel(c)):
            raise ValueError('celula: argumentos invalidos')

        return self.tabuleiro[c.linha() - 1][c.coluna() - 1]

    def preenche_celula(self, c, e):
        if not (isinstance(c, Coordenada) and self.coordenada_compativel(c) and
                isinstance(e, Celula)):
            raise ValueError('preenche_celula: argumentos invalidos')

        self.tabuleiro[c.linha() - 1][c.coluna() - 1] = e
        return copy.deepcopy(self)

    def __eq__(self, outro):
        if not isinstance(outro, Tabuleiro):
            ValueError('Tabuleiro =: argumentos invalidos')

        return self.conf == outro.conf and self.tabuleiro == outro.tabuleiro

    def coordenada_compativel(self, c):
        if not isinstance(c, Coordenada):
            raise ValueError('coordenada_compativel: argumentos invalidos')

        dim = self.dimensoes()

        return 1 <= c.linha() <= dim[0] and 1 <= c.coluna() <= dim[1]


class Jogada():
    def __init__(self, coordenada, valor):
        if not(isinstance(coordenada, Coordenada) and
               isinstance(valor, Celula)) or valor == Celula.BRANCO:
            raise ValueError("Jogada: argumentos invalidos")

        self.coordenada = coordenada
        self.valor = valor

    def __eq__(self, outra):
        if not isinstance(outra, Jogada):
            raise ValueError("Jogada =: argumentos invalidos")

        return self.coordenada == outra.coordenada and \
            self.valor == outra.valor

    def __str__(self):
        return f"{self.coordenada} --> {self.valor.name}"


def le_tabuleiro(filename):
    with open(filename) as file:
        read_data = eval(file.read())  # DANGER: This assumes the file is safe.

    if not verifica_conf(read_data):
        raise ValueError("le_tabuleiro: argumentos invalidos")

    return read_data


def pede_jogada(tabuleiro):

    if not isinstance(tabuleiro, Tabuleiro):
        raise ValueError('pede_jogada: argumentos invalidos')

    dim = tabuleiro.dimensoes()

    print("Introduza uma jogada")

    jogada_valida = False

    while not jogada_valida:

        try:
            linha = int(input(f"- Introduza a linha (1 a {dim[0]}): "))
            coluna = int(input(f"- Introduza a coluna (1 a {dim[1]}): "))
        except ValueError:
            continue

        valor = input("- Introduza BRANCO ou CAIXA: ")

        coordenada = Coordenada(linha, coluna)
        if not (tabuleiro.coordenada_compativel(coordenada) and
                (valor == "BRANCO" or valor == "CAIXA")):
            print("Jogada invalida")
            continue

        jogada_valida = True

    return Jogada(coordenada, Celula[valor])


def verifica_conf(conf):

    def verifica_conf_desc(dim, x):

        if not (isinstance(x, tuple) and all(isinstance(v, int) and v >= 1
                for v in x)):
            return False

        s = sum(x)
        if s + len(x) - 1 > dim:
            return False
        return s

    dim_linhas = len(conf[0])
    dim_colunas = len(conf[1])

    if not (isinstance(conf, tuple) and len(conf) == 2 and
       isinstance(conf[0], tuple) and isinstance(conf[1], tuple) and
       dim_linhas == dim_colunas):
        return False

    total = [0, 0]
    for i in range(0, 2):
        for x in conf[i]:
            total[i] += verifica_conf_desc(len(conf[0]), x)
            if not total[i]:
                return False
    return total[0] == total[1]
