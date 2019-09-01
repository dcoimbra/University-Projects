import numpy

CIRCULOS = (('Aveiro', 16),
            ('Beja', 3),
            ('Braga', 19),
            ('Braganca', 3),
            ('Castelo Branco', 4),
            ('Coimbra', 9),
            ('Evora', 3),
            ('Faro', 9),
            ('Guarda', 4),
            ('Leiria', 10),
            ('Lisboa', 47),
            ('Portalegre', 2),
            ('Porto', 39),
            ('Santarem', 9),
            ('Setubal',  18),
            ('Viana do Castelo', 6),
            ('Vila Real', 5),
            ('Viseu', 9),
            ('Acores', 5),
            ('Madeira', 6),
            ('Europa', 2),
            ('Fora da Europa', 2))

PARTIDOS = (('Partido Democratico Republicano', 'PDR'),
            ('CDU - Coligacao Democratica Unitaria', 'PCP-PEV'),
            ('Portugal a Frente', 'PPD/PSD-CDS/PP'),
            ('Partido da Terra', 'MPT'),
            ('LIVRE/Tempo de Avancar', 'L/TDA'),
            ('Pessoas-Animais-Natureza', 'PAN'),
            ('Agir', 'PTP-MAS'),
            ('Juntos pelo Povo', 'JPP'),
            ('Partido Nacional Renovador', 'PNR'),
            ('Partido Popular Monarquico', 'PPM'),
            ('Nos, Cidadaos!', 'NC'),
            ('Partido Comunista dos Trabalhadores Portugueses', 'PCTP/MRPP'),
            ('Partido Socialista', 'PS'),
            ('Bloco de Esquerda', 'B.E.'),
            ('Partido Unido dos Reformados e Pensionistas', 'PURP'))


def selecciona_deputado(eleitos, maximos):

    seleccionado = maximos[0]

    for i in maximos:
        if eleitos[i] < eleitos[seleccionado]:
            seleccionado = i

    return seleccionado


def mandatos(nr_mandatos: int, nr_votos: tuple) -> tuple:

    nr_votos = numpy.array(object=nr_votos)

    eleitos = numpy.zeros(shape=nr_votos.size, dtype=numpy.int64)
    divisores = numpy.ones(shape=nr_votos.size, dtype=numpy.int64)

    while (nr_mandatos > 0):
        quocientes = numpy.divide(nr_votos, divisores)
        selecionado = numpy.argmax(a=quocientes)

        if (type(selecionado) is numpy.ndarray):
            selecionado = selecciona_deputado(nr_votos, selecionado)

        eleitos[selecionado] += 1
        divisores[selecionado] += 1
        nr_mandatos -= 1

    return tuple(eleitos)


def assembleia(votacoes):

    deputados = numpy.zeros(len(PARTIDOS), dtype=numpy.int64)

    for i in range(len(votacoes)):
        deputados = numpy.add(deputados, numpy.array(mandatos(CIRCULOS[i][1],
                                                              votacoes[i])))

    return tuple(deputados)


def max_mandatos(votacoes):

    constituicao_assembleia = numpy.array(assembleia(votacoes))

    idx = numpy.argmax(constituicao_assembleia)

    if type(idx) is numpy.ndarray:
        return "Empate tecnico"

    return PARTIDOS[idx][1] + "\t" + PARTIDOS[idx][0]
