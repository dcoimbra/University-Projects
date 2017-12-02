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
  design varchar(50) UNIQUE,
  categoria varchar(20) REFERENCES categoria(nome),
  forn_primario integer REFERENCES fornecedor(nif) NOT NULL,
  data date,
  CHECK (ean > 999999999999 and ean <= 9999999999999),
  PRIMARY KEY (ean)
);

CREATE TABLE fornece_sec (
  nif integer REFERENCES fornecedor(nif) NOT NULL,
  ean bigint REFERENCES produto(ean) ON DELETE CASCADE,
  PRIMARY KEY (nif, ean)
);

CREATE TABLE corredor (
  nro integer,
  largura integer,
  PRIMARY KEY(nro)
);

CREATE TABLE prateleira (
  nro integer REFERENCES corredor(nro),
  lado varchar(8),
  CHECK (lado IN ('esquerdo', 'direito')),
  altura varchar(8),
  CHECK (altura IN ('chao', 'medio', 'superior')),
  PRIMARY KEY (nro, lado, altura)
);

CREATE TABLE planograma (
  ean bigint REFERENCES produto(ean) ON DELETE CASCADE,
  nro integer,
  lado varchar(8),
  altura varchar(8),
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
  nro integer,
  lado varchar(8),
  altura varchar(8),
  operador varchar(50),
  instante timestamp,
  unidades integer,
  PRIMARY KEY (ean, nro, lado, altura, operador, instante),
  FOREIGN KEY (operador, instante) REFERENCES evento_reposicao(operador, instante),
  FOREIGN KEY (ean, nro, lado, altura) REFERENCES planograma(ean, nro, lado, altura) ON DELETE CASCADE
);
