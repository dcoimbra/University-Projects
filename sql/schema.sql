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
  PRIMARY KEY (super_categoria, categoria)
);

CREATE TABLE fornecedor (
  nif integer,
	nome varchar(20),
	PRIMARY KEY (nif)
);

CREATE TABLE produto (
	ean integer,
	design varchar(50),
  categoria varchar(20) REFERENCES categoria(nome),
  forn_primario integer REFERENCES fornecedor(nif),
  data date,
	PRIMARY KEY (ean),
	UNIQUE (design)
);

CREATE TABLE fornece_sec (
  nif integer REFERENCES fornecedor(nif),
  ean integer REFERENCES produto(ean) ON DELETE CASCADE,
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
  altura varchar(20),
  PRIMARY KEY (nro, lado, altura)
);

CREATE TABLE planograma (
  ean integer REFERENCES produto(ean) ON DELETE CASCADE,
  nro integer,
  lado varchar(20),
  altura varchar(20),
  face integer,
  unidades integer,
  loc integer,
  PRIMARY KEY(ean, nro, lado, altura),
  FOREIGN KEY(nro, lado, altura) REFERENCES prateleira(nro, lado, altura)
);

CREATE TABLE evento_reposicao (
  operador varchar(50),
  instante timestamp,
  PRIMARY KEY (operador, instante)
);

CREATE TABLE reposicao (
  ean integer,
  nro integer,
  lado varchar(20),
  altura varchar(20),
  operador varchar(50),
  instante timestamp,
  unidades integer,
  PRIMARY KEY (ean, nro, lado, altura, operador, instante),
  FOREIGN KEY (operador, instante) REFERENCES evento_reposicao(operador, instante),
  FOREIGN KEY (ean, nro, lado, altura) REFERENCES planograma(ean, nro, lado, altura) ON DELETE CASCADE
);
