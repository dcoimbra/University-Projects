-- SELECT * FROM categoria;
-- SELECT * FROM super_categoria;
-- SELECT * FROM categoria_simples;
-- SELECT * FROM constituida;
-- SELECT * FROM fornecedor;
-- SELECT * FROM produto;
-- SELECT * FROM fornece_sec;
-- SELECT * FROM corredor;
-- SELECT * FROM prateleira;
-- SELECT * FROM planograma;
SELECT * FROM evento_reposicao;
SELECT * FROM reposicao;

-- a)
/*SELECT nome, nif, ean, categoria
FROM (SELECT nif, nome, ean, categoria
      FROM (produto NATURAL JOIN fornecedor) AS f1
      UNION
      SELECT nif, nome, produto.ean, categoria
            FROM (fornecedor NATURAL JOIN fornece_sec) AS f2
            JOIN produto
            ON f2.ean = produto.ean) AS af
GROUP BY nif
HAVING count(af.categoria) >= ALL(SELECT count(categoria)
                                  FROM (produto NATURAL JOIN fornecedor) AS f1
                                  UNION
                                  SELECT nif, nome, produto.ean, categoria
                                  FROM (fornecedor NATURAL JOIN fornece_sec) AS f2
                                  JOIN produto
                                  ON f2.ean = produto.ean); */

-- b)
SELECT *
FROM ((SELECT categoria, forn_primario
      FROM categoria_simples c, produto p
      WHERE c.nome = p.categoria) AS cat
      INNER JOIN fornecedor
      ON cat.forn_primario = fornecedor.nif);











