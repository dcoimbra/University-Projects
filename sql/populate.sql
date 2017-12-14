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


INSERT INTO fornecedor(nif, nome) VALUES (123455678, 'Umberto & Filhos');
INSERT INTO fornecedor(nif, nome) VALUES (222222222, 'Dolores & Filhas');
INSERT INTO fornecedor(nif, nome) VALUES (333333333, 'Teresa SA');
INSERT INTO fornecedor(nif, nome) VALUES (444444444, 'Quim Electro');

INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
     1111111111111,
     'Laranja do Algarve',
     'fruta',
     123455678,
     '2017-01-22'
  );
  
INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
    2222222222222,
    'Gelado de Chocolate',
    'gelados',
    222222222,
    '2017-01-23'
  );

INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
    3333333333333,
    'Batedeira para bolos',
    'cozinha',
    333333333,
    '2017-01-23'
  );

INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
     4444444444444,
     'Pera Rocha',
     'fruta',
     123455678,
     '2017-01-22'
  );

INSERT INTO produto(ean, design, categoria, forn_primario, data)
  VALUES (
     5555555555555,
     'Mousse de Manga',
     'sobremesas',
     123455678,
     '2017-01-22'
  );



INSERT INTO fornece_sec(nif, ean) VALUES (444444444, 1111111111111);
INSERT INTO fornece_sec(nif, ean) VALUES (222222222, 3333333333333);
INSERT INTO fornece_sec(nif, ean) VALUES (333333333, 1111111111111);
INSERT INTO fornece_sec(nif, ean) VALUES (222222222, 4444444444444);
INSERT INTO fornece_sec(nif, ean) VALUES (444444444, 3333333333333);
INSERT INTO fornece_sec(nif, ean) VALUES (222222222, 5555555555555);

INSERT INTO corredor(nro, largura) VALUES (1, 2);
INSERT INTO corredor(nro, largura) VALUES (2, 2);

INSERT INTO prateleira(nro, lado, altura) VALUES (
    1,
    'esquerdo',
    'superior'
  );

INSERT INTO prateleira(nro, lado, altura) VALUES (
    1,
    'direito',
    'superior'
  );

INSERT INTO prateleira(nro, lado, altura) VALUES (
    1,
    'esquerdo',
    'medio'
  );

INSERT INTO prateleira(nro, lado, altura) VALUES (
    1,
    'direito',
    'medio'
  );

INSERT INTO prateleira(nro, lado, altura)
  VALUES (
    2,
    'esquerdo',
    'superior'
  );

INSERT INTO prateleira(nro, lado, altura)
  VALUES (
    2,
    'direito',
    'superior'
  );

INSERT INTO prateleira(nro, lado, altura)
  VALUES (
    2,
    'esquerdo',
    'medio'
  );

INSERT INTO prateleira(nro, lado, altura)
  VALUES (
    2,
    'direito',
    'medio'
  );

INSERT INTO planograma(ean, nro, lado, altura, face, unidades, loc)
  VALUES (
      1111111111111,
      1,
      'esquerdo',
      'superior',
      1,
      40,
      1
    );

INSERT INTO planograma(ean, nro, lado, altura, face, unidades, loc)
  VALUES (
      2222222222222,
      1,
      'esquerdo',
      'superior',
      2,
      40,
      2
    );

INSERT INTO planograma(ean, nro, lado, altura, face, unidades, loc)
  VALUES (
      3333333333333,
      1,
      'direito',
      'superior',
      1,
      5,
      2
    );

INSERT INTO planograma(ean, nro, lado, altura, face, unidades, loc)
  VALUES (
      4444444444444,
      2,
      'esquerdo',
      'superior',
      1,
      40,
      2
    );

INSERT INTO planograma(ean, nro, lado, altura, face, unidades, loc)
  VALUES (
      5555555555555,
      2,
      'esquerdo',
      'medio',
      1,
      40,
      2
    );

INSERT INTO planograma(ean, nro, lado, altura, face, unidades, loc)
VALUES (
  5555555555555,
  1,
  'esquerdo',
  'superior',
  1,
  40,
  2
);

INSERT INTO evento_reposicao(operador, instante)
  VALUES (
    'joao',
    '2017-11-02 15:43'
  );

INSERT INTO evento_reposicao(operador, instante)
  VALUES (
    'alberto',
    '2017-11-04 18:32'
  );

INSERT INTO evento_reposicao(operador, instante)
VALUES (
  'david',
  '2017-10-02 15:43'
);

INSERT INTO reposicao(ean, nro, lado, altura, operador, instante, unidades)
  VALUES (
    1111111111111,
    1,
	 'esquerdo',
	 'superior',
    'joao',
    '2017-11-02 15:43',
    20
  );

INSERT INTO reposicao(ean, nro, lado, altura, operador, instante, unidades)
VALUES (
  5555555555555,
  1,
  'esquerdo',
  'superior',
  'david',
  '2017-10-02 15:43',
  20
);

INSERT INTO reposicao(ean, nro, lado, altura, operador, instante, unidades)
  VALUES (
    2222222222222,
    1,
    'esquerdo',
    'superior',
    'joao',
    '2017-11-02 15:43',
    20
  );

INSERT INTO reposicao(ean, nro, lado, altura, operador, instante, unidades)
  VALUES (
    2222222222222,
    1,
    'esquerdo',
    'superior',
    'alberto',
    '2017-11-04 18:32',
    20
  );
  
INSERT INTO reposicao(ean, nro, lado, altura, operador, instante, unidades)
  VALUES (
    3333333333333,
    1,
	 'direito',
	 'superior',
    'joao',
    '2017-11-02 15:43',
    1
  );

-- Teste de indices

-- correr primeiro a que cria fornecedores, e de seguida a que cria produtos -> com a mesma quantity <-

-- criar mais fornecedores
CREATE OR REPLACE FUNCTION add_fornecedores(
  forneprefix varchar,
  quantity integer)
  RETURNS void
LANGUAGE 'plpgsql'
AS $$

DECLARE
  i integer;

BEGIN
  i := 0;
  WHILE(i <= quantity)
  loop
    INSERT INTO fornecedor(nif,nome) VALUES (200000000 + i, concat(forneprefix, i));

    i := i + 1;
  end loop;
END $$;

-- criar produtos
CREATE OR REPLACE FUNCTION add_produtos(
  eanbase bigint,
  prodprefix varchar,
  quantity integer)
  RETURNS void
LANGUAGE 'plpgsql'

AS $$

DECLARE
  i integer;
  cat text[];
  catlen integer;

BEGIN
  i := 1;
  cat := ARRAY(SELECT nome FROM categoria);
  catlen := array_length(cat, 1);
  WHILE(i <= quantity)
  loop
    INSERT INTO produto(ean, design, categoria, forn_primario, data) VALUES (
      eanbase + i,
      concat(prodprefix, i),
      cat[i%catlen+1],
      200000000 + i,
      current_date);
    INSERT INTO fornece_sec(nif, ean) VALUES(200000000 + i - 1, eanbase + i);
    i := i + 1;
  end loop;
END $$;

-- Chamadas as funcoes para testar indices

/* DO $$ BEGIN
  PERFORM add_fornecedores('f', 10000);
  PERFORM add_produtos(1111111111111, 'p', 10000);
END $$; */
