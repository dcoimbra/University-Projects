DROP TABLE IF EXISTS categoria, categoria_simples, super_categoria,constituida, fornecedor, produto, fornecedor, fornece_sec, corredor, prateleira, planograma, evento_reposicao, reposicao;

CREATE TABLE categoria (
  nome varchar(20),
  PRIMARY KEY (nome)
);

CREATE TABLE categoria_simples (
  nome varchar(20) REFERENCES categoria(nome),
  PRIMARY KEY (nome)
);

CREATE TABLE super_categoria (
  nome varchar(20) REFERENCES categoria(nome),
  PRIMARY KEY (nome)
);

CREATE TABLE constituida (
  super_categoria varchar(20) REFERENCES super_categoria(nome),
  categoria varchar(20) REFERENCES categoria(nome),
  CHECK (super_categoria != categoria),
  PRIMARY KEY (super_categoria, categoria),
  UNIQUE(categoria)
);

CREATE TABLE fornecedor (
  nif integer,
  CHECK (nif > 99999999 and nif <= 999999999),
	nome varchar(20),
	PRIMARY KEY (nif)
);

CREATE TABLE produto (
	ean bigint,
  CHECK (ean > 999999999999 and ean <= 9999999999999),
	design varchar(50),
  categoria varchar(20) REFERENCES categoria(nome),
  forn_primario integer REFERENCES fornecedor(nif) NOT NULL,
  CHECK (forn_primario > 99999999 and forn_primario <= 999999999),
  data date,
	PRIMARY KEY (ean),
	UNIQUE (design)
);

CREATE TABLE fornece_sec (
  nif integer REFERENCES fornecedor(nif) NOT NULL,
  CHECK (nif > 99999999 and nif <= 999999999),
  ean bigint REFERENCES produto(ean) ON DELETE CASCADE,
  CHECK (ean > 999999999999 and ean <= 9999999999999),
  PRIMARY KEY (nif, ean)
);

CREATE TABLE corredor (
  nro integer,
  largura integer,
  PRIMARY KEY(nro)
);

CREATE TABLE prateleira (
  nro integer REFERENCES corredor(nro),
  lado varchar(20),
  altura varchar(10),
  CHECK (altura IN ('chao', 'medio', 'superior')),
  PRIMARY KEY (nro, lado, altura)
);

CREATE TABLE planograma (
  ean bigint REFERENCES produto(ean) ON DELETE CASCADE,
  CHECK (ean > 999999999999 and ean <= 9999999999999),
  nro integer,
  lado varchar(10),
  CHECK (lado IN ('esquerdo', 'direito')),
  altura varchar(10),
  CHECK (altura IN ('chao', 'medio', 'superior')),
  face integer,
  unidades integer,
  loc integer,
  PRIMARY KEY(ean, nro, lado, altura),
  FOREIGN KEY(nro, lado, altura) REFERENCES prateleira(nro, lado, altura)
);

CREATE TABLE evento_reposicao (
  operador varchar(50),
  instante timestamp,
  CHECK (instante <= CURRENT_TIMESTAMP),
  PRIMARY KEY (operador, instante)
);

CREATE TABLE reposicao (
  ean bigint,
  CHECK (ean > 999999999999 and ean <= 9999999999999),
  nro integer,
  lado varchar(20),
  altura varchar(10),
  CHECK (altura IN ('chao', 'medio', 'superior')),
  operador varchar(50),
  instante timestamp,
  CHECK (instante <= CURRENT_TIMESTAMP),
  unidades integer,
  PRIMARY KEY (ean, nro, lado, altura, operador, instante),
  FOREIGN KEY (operador, instante) REFERENCES evento_reposicao(operador, instante),
  FOREIGN KEY (ean, nro, lado, altura) REFERENCES planograma(ean, nro, lado, altura) ON DELETE CASCADE
);
