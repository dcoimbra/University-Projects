INSERT INTO categoria(nome) VALUES ('fruta');
INSERT INTO categoria(nome) VALUES ('brinquedos');
INSERT INTO categoria(nome) VALUES ('congelados');
INSERT INTO categoria(nome) VALUES ('mobilia');
INSERT INTO categoria(nome) VALUES ('frutos vermelhos');
INSERT INTO categoria(nome) VALUES ('bebidas');
INSERT INTO categoria(nome) VALUES ('peixe');
INSERT INTO categoria(nome) VALUES ('sobremesas');
INSERT INTO categoria(nome) VALUES ('cozinha');
INSERT INTO categoria(nome) VALUES ('bebidas com gas');
INSERT INTO categoria(nome) VALUES ('bolos');

INSERT INTO categoria_simples(nome) VALUES ('brinquedos');
INSERT INTO categoria_simples(nome) VALUES ('frutos vermelhos');
INSERT INTO categoria_simples(nome) VALUES ('peixe');
INSERT INTO categoria_simples(nome) VALUES ('bebidas com gas');
INSERT INTO categoria_simples(nome) VALUES ('bolos');
INSERT INTO categoria_simples(nome) VALUES ('fruta');

INSERT INTO super_categoria(nome) VALUES ('congelados');
INSERT INTO super_categoria(nome) VALUES ('bebidas');
INSERT INTO super_categoria(nome) VALUES ('mobilia');
INSERT INTO super_categoria(nome) VALUES ('sobremesas');
INSERT INTO super_categoria(nome) VALUES ('cozinha');

INSERT INTO constituida(super_categoria, categoria) VALUES ('congelados', 'peixe');
INSERT INTO constituida(super_categoria, categoria) VALUES ('congelados', 'sobremesas');
INSERT INTO constituida(super_categoria, categoria) VALUES ('bebidas', 'bebidas com gas');
INSERT INTO constituida(super_categoria, categoria) VALUES ('mobilia', 'cozinha');

INSERT INTO fornecedor(nif, nome)
  VALUES (
    111,
    'um'
  );

INSERT INTO fornecedor(nif, nome)
  VALUES (
    222,
    'dois'
  );

INSERT INTO fornecedor(nif, nome)
  VALUES (
    333,
    'tres'
  );

INSERT INTO fornecedor(nif, nome)
  VALUES (
    444,
    'quatro'
  );

INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
     1111,
     'maca',
     'fruta',
     111,
     '2017-01-22'
  );
  
INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
    2222,
    'gelado',
    'sobremesas',
    222,
    '2017-01-23'
  );

INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
    3333,
    'cadeira',
    'mobilia',
    333,
    '2017-01-23'
  );

INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
     4444,
     'pera',
     'fruta',
     111,
     '2017-01-22'
  );

INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
     5555,
     'morangos com chantily',
     'sobremesas',
     111,
     '2017-01-22'
  );

INSERT INTO fornece_sec(nif, ean)
  VALUES (
    444,
    1111
  );

INSERT INTO fornece_sec(nif, ean)
  VALUES (
    222,
    3333
  );

INSERT INTO fornece_sec(nif, ean)
  VALUES (
    333,
    1111
  );

INSERT INTO fornece_sec(nif, ean)
  VALUES (
    222,
    4444
  );

INSERT INTO fornece_sec(nif, ean)
  VALUES (
    444,
    3333
  );
  
INSERT INTO fornece_sec(nif, ean)
  VALUES (
    222,
    5555
  );

INSERT INTO corredor(nro, largura)
  VALUES (
    1,
    1
  );

INSERT INTO corredor(nro, largura)
  VALUES (
    2,
    2
  );

INSERT INTO prateleira(nro, lado, altura)
  VALUES (
    1,
    'esquerdo',
    'cima'
  );

INSERT INTO prateleira(nro, lado, altura)
  VALUES (
    2,
    'esquerdo',
    'cima'
  );

INSERT INTO planograma(ean, nro, lado, altura, face, unidades, loc)
  VALUES (
      1111,
      1,
      'esquerdo',
      'cima',
      1,
      3,
      1
    );

INSERT INTO planograma(ean, nro, lado, altura, face, unidades, loc)
  VALUES (
      2222,
      1,
      'esquerdo',
      'cima',
      2,
      4,
      2
    );

INSERT INTO evento_reposicao(operador, instante)
  VALUES (
    'joao',
    '2017-11-02 00:00'
  );

INSERT INTO evento_reposicao(operador, instante)
  VALUES (
    'alberto',
    '2017-11-04 00:00'
  );

INSERT INTO reposicao(ean, nro, lado, altura, operador, instante, unidades)
  VALUES (
    2222,
    1,
    'esquerdo',
    'cima',
    'joao',
    '2017-11-02 00:00',
    2
  );

INSERT INTO reposicao(ean, nro, lado, altura, operador, instante, unidades)
  VALUES (
    2222,
    1,
    'esquerdo',
    'cima',
    'alberto',
    '2017-11-04 00:00',
    2
  );









