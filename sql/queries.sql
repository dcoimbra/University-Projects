-- c)
SELECT produto.ean
FROM produto
LEFT JOIN reposicao
ON produto.ean = reposicao.ean
WHERE reposicao.ean IS NULL;

-- d)
SELECT ean
FROM fornece_sec
GROUP BY ean
HAVING count(nif) > 10;

-- e)
SELECT ean
FROM reposicao
GROUP BY ean
HAVING count(DISTINCT operador) = 1;