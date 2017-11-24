-- a) Qual o nome do fornecedor que forneceu o maior numero de categorias? Note que pode ser mais do que um fornecedor.
WITH forn_by_cat(nif, num_cat) AS
(SELECT nif, count(distinct categoria) 
      FROM (SELECT nif, categoria 																	 
					FROM fornece_sec fs INNER JOIN produto p ON fs.ean = p.ean				 
					UNION 																					 
					SELECT p.forn_primario as nif, p.categoria FROM produto p) AS all_forn
      GROUP BY nif)						   -- 
SELECT f.nome
	FROM forn_by_cat INNER JOIN fornecedor f ON forn_by_cat.nif = f.nif
WHERE num_cat = (SELECT max(num_cat) FROM forn_by_cat)



-- b) Quais os fornecedores primarios (nome e nif) que forneceram produtos de todas as categorias simples?
SELECT f.nome, f.nif
FROM categoria_simples cs INNER JOIN produto pr ON cs.nome = pr.categoria
	INNER JOIN fornecedor f ON pr.forn_primario = f.nif
GROUP BY f.nif
HAVING count (DISTINCT categoria) = (SELECT count (nome) FROM categoria_simples)



-- c) Quais os produtos (ean) que nunca foram repostos?
SELECT produto.ean
FROM produto LEFT JOIN reposicao ON produto.ean = reposicao.ean
WHERE reposicao.ean IS NULL;



-- d) Quais os produtos (ean) com um numero de fornecedores secundarios superior a 10?
SELECT ean
FROM fornece_sec
GROUP BY ean
HAVING count(nif) > 10;



-- e) Quais os produtos (ean) que foram repostos sempre pelo mesmo operador?
SELECT ean
FROM reposicao
GROUP BY ean
HAVING count(DISTINCT operador) = 1;