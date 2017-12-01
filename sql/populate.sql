INSERT INTO categoria(nome) VALUES ('gelados');
INSERT INTO categoria(nome) VALUES ('frutos vermelhos');
INSERT INTO categoria(nome) VALUES ('peixe');
INSERT INTO categoria(nome) VALUES ('refrigerantes');
INSERT INTO categoria(nome) VALUES ('pao');
INSERT INTO categoria(nome) VALUES ('bolos');
INSERT INTO categoria(nome) VALUES ('cervejas');
INSERT INTO categoria(nome) VALUES ('fruta');
INSERT INTO categoria(nome) VALUES ('congelados');
INSERT INTO categoria(nome) VALUES ('bebidas');
INSERT INTO categoria(nome) VALUES ('bebidas com gas');
INSERT INTO categoria(nome) VALUES ('sobremesas');
INSERT INTO categoria(nome) VALUES ('cozinha');

INSERT INTO categoria_simples(nome) VALUES ('gelados');
INSERT INTO categoria_simples(nome) VALUES ('frutos vermelhos');
INSERT INTO categoria_simples(nome) VALUES ('peixe');
INSERT INTO categoria_simples(nome) VALUES ('refrigerantes');
INSERT INTO categoria_simples(nome) VALUES ('pao');
INSERT INTO categoria_simples(nome) VALUES ('bolos');
INSERT INTO categoria_simples(nome) VALUES ('cervejas');

INSERT INTO super_categoria(nome) VALUES ('fruta');
INSERT INTO super_categoria(nome) VALUES ('congelados');
INSERT INTO super_categoria(nome) VALUES ('bebidas');
INSERT INTO super_categoria(nome) VALUES ('bebidas com gas');
INSERT INTO super_categoria(nome) VALUES ('sobremesas');
INSERT INTO super_categoria(nome) VALUES ('cozinha');


INSERT INTO constituida(super_categoria, categoria) VALUES ('bebidas', 'bebidas com gas');
INSERT INTO constituida(super_categoria, categoria) VALUES ('bebidas com gas', 'refrigerantes');
INSERT INTO constituida(super_categoria, categoria) VALUES ('bebidas com gas', 'cervejas');
INSERT INTO constituida(super_categoria, categoria) VALUES ('congelados', 'peixe');
INSERT INTO constituida(super_categoria, categoria) VALUES ('cozinha', 'sobremesas');
INSERT INTO constituida(super_categoria, categoria) VALUES ('sobremesas', 'fruta');
INSERT INTO constituida(super_categoria, categoria) VALUES ('sobremesas', 'bolos');
INSERT INTO constituida(super_categoria, categoria) VALUES ('sobremesas', 'gelados');
INSERT INTO constituida(super_categoria, categoria) VALUES ('fruta', 'frutos vermelhos');


INSERT INTO fornecedor(nif, nome) VALUES (111111111, 'um');
INSERT INTO fornecedor(nif, nome) VALUES (222222222, 'dois');
INSERT INTO fornecedor(nif, nome) VALUES (333333333, 'tres');
INSERT INTO fornecedor(nif, nome) VALUES (444444444, 'quatro');

INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
     1111111111111,
     'maca',
     'fruta',
     111111111,
     '2017-01-22'
  );
  
INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
    2222222222222,
    'gelado de chocolate',
    'gelados',
    222222222,
    '2017-01-23'
  );

INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
    3333333333333,
    'batedeira para bolos',
    'cozinha',
    333333333,
    '2017-01-23'
  );

INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
     4444444444444,
     'pera',
     'fruta',
     111111111,
     '2017-01-22'
  );

INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
     5555555555555,
     'morangos com chantily',
     'sobremesas',
     111111111,
     '2017-01-22'
  );

INSERT INTO fornece_sec(nif, ean) VALUES (444444444, 1111111111111);
INSERT INTO fornece_sec(nif, ean) VALUES (222222222, 3333333333333);
INSERT INTO fornece_sec(nif, ean) VALUES (333333333, 1111111111111);
INSERT INTO fornece_sec(nif, ean) VALUES (222222222, 4444444444444);
INSERT INTO fornece_sec(nif, ean) VALUES (444444444, 3333333333333);
INSERT INTO fornece_sec(nif, ean) VALUES (222222222, 5555555555555);

INSERT INTO corredor(nro, largura) VALUES (1, 1);
INSERT INTO corredor(nro, largura) VALUES (2, 2);

INSERT INTO prateleira(nro, lado, altura) VALUES (
    1,
    'esquerdo',
    'superior'
  );

INSERT INTO prateleira(nro, lado, altura)
  VALUES (
    2,
    'esquerdo',
    'superior'
  );

INSERT INTO planograma(ean, nro, lado, altura, face, unidades, loc)
  VALUES (
      1111111111111,
      1,
      'esquerdo',
      'superior',
      1,
      3,
      1
    );

INSERT INTO planograma(ean, nro, lado, altura, face, unidades, loc)
  VALUES (
      2222222222222,
      1,
      'esquerdo',
      'superior',
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
    2222222222222,
    1,
    'esquerdo',
    'superior',
    'joao',
    '2017-11-02 00:00',
    2
  );

INSERT INTO reposicao(ean, nro, lado, altura, operador, instante, unidades)
  VALUES (
    2222222222222,
    1,
    'esquerdo',
    'superior',
    'alberto',
    '2017-11-04 00:00',
    2
  );
