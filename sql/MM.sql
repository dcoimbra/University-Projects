DROP TABLE IF EXISTS facts_table, d_produto, d_tempo;

CREATE TABLE d_produto(
  cean BIGINT,
  categoria varchar(20),
  nif_fornecedor_principal INTEGER,
  PRIMARY KEY (cean)
);

CREATE TABLE d_tempo(
  time_id INTEGER,
  dia INTEGER NOT NULL ,
  mes varchar(10) NOT NULL ,
  ano INTEGER NOT NULL,
  PRIMARY KEY (time_id)
);

CREATE TABLE facts_table(
  cean BIGINT,
  FOREIGN KEY (cean) REFERENCES d_produto(cean),
  time_id INTEGER,
  FOREIGN KEY (time_id) REFERENCES d_tempo(time_id),
  n_reposicoes INTEGER
);


INSERT INTO d_produto (cean, categoria, nif_fornecedor_principal)
SELECT ean, categoria, forn_primario
FROM produto;

INSERT INTO d_tempo (time_id, dia, mes, ano)
SELECT random(), extract(day from instante), extract (month from instante), extract (year from instante)
FROM reposicao;

INSERT INTO facts_table(cean, time_id, n_reposicoes)
SELECT ean, random(), unidades
FROM reposicao;