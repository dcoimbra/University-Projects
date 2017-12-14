DROP TABLE IF EXISTS facts_table, d_produto, d_tempo;

CREATE TABLE d_produto(
  cean BIGINT,
  categoria varchar(20),
  nif_fornecedor_principal INTEGER,
  PRIMARY KEY (cean)
);

CREATE TABLE d_tempo(
  dia INTEGER NOT NULL ,
  mes varchar(10) NOT NULL ,
  ano INTEGER NOT NULL,
  PRIMARY KEY (dia, mes, ano)
);

CREATE TABLE facts_table(
  cean BIGINT,
  FOREIGN KEY (cean) REFERENCES d_produto(cean),
  dia INTEGER NOT NULL ,
  mes varchar(10) NOT NULL ,
  ano INTEGER NOT NULL,
  FOREIGN KEY (dia, mes, ano) REFERENCES d_tempo(dia, mes, ano),
  n_reposicoes INTEGER,
  PRIMARY KEY(cean, dia, mes, ano)
);


INSERT INTO d_produto (cean, categoria, nif_fornecedor_principal)
SELECT ean, categoria, forn_primario
FROM produto;

-- -------------------------------------------------------------------------------------------------------------
DO $$ BEGIN
  PERFORM "polulate"();
END $$;

DROP FUNCTION IF EXISTS polulate();

CREATE FUNCTION polulate()
  returns void
AS
$$
DECLARE date DATE := '2017-01-01';
BEGIN
  while date < '2018-12-31'
  loop
    INSERT INTO d_tempo (dia, mes, ano) VALUES (
      extract(day from date),
      extract(month from date),
      extract(year from date)
    );
    date := date + INTERVAL '1 day';
  end loop;
END
$$
language plpgsql;

-- -----------------------------------------------------------------------------------------------------------

INSERT INTO facts_table(cean, dia, mes, ano, n_reposicoes)
SELECT ean, extract(day from instante), extract (month from instante), extract (year from instante), unidades
FROM reposicao;

-- -----------------------------------------------------------------------------------------------------------------
(SELECT count(n_reposicoes), categoria, mes, ano
  FROM facts_table, d_produto
  WHERE facts_table.cean = d_produto.cean and nif_fornecedor_principal = '123455678'
  GROUP BY categoria, mes, ano)

UNION

(SELECT count(n_reposicoes), categoria, null, ano
  FROM facts_table, d_produto
  WHERE facts_table.cean = d_produto.cean and nif_fornecedor_principal = '123455678'
  GROUP BY categoria, ano)

UNION

(SELECT count(n_reposicoes), categoria, null, null
 FROM facts_table, d_produto
 WHERE facts_table.cean = d_produto.cean and nif_fornecedor_principal = '123455678'
 GROUP BY categoria);

-- ----------------------------------------------------------------------------------------------------------------
