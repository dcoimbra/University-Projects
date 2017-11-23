<html>
    <body>
        <h3>Produtos</h3>
        <?php
        try
        {
            $host = "db.ist.utl.pt";
            $user ="ist426008";
            $password = "northernlights";
            $dbname = $user;

            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            $sql = "SELECT * FROM produto;";

            $result = $db->query($sql);

            echo("<table border=\"0\" cellspacing=\"5\">\n");

            echo("<tr>\n
                    <th>EAN</th>\n
                    <th>Designacao</th>\n
                    <th>Categoria</th>\n
                    <th>Fornecedor primario</th>\n
                    <th>Data</th>\n
                  </tr>\n");

            foreach($result as $row)
            {
                echo("<tr>\n");
                echo("<td>{$row['ean']}</td>\n");
                echo("<td>{$row['design']}</td>\n");
                echo("<td>{$row['categoria']}</td>\n");
                echo("<td>{$row['forn_primario']}</td>\n");
                echo("<td>{$row['data']}</td>\n");
                echo("<td><a href=\"design_d.php?ean={$row['ean']}\">Editar designacao</a></td>\n");
                echo("<td><a href=\"update_prod.php?ean={$row['ean']}&tipo=remover\">Remover produto</a></td>\n");
                echo("<td><a href=\"listar_reposicoes.php?ean={$row['ean']}\">Listar eventos de reposicao</a></td>\n");
                echo("</tr>\n");
            }
            echo("<tr><td><a href=\"inserir_produto_b.php\">Novo produto</a></td></tr>");
            echo("</table>\n");
        }
        catch (PDOException $e)
        {
            echo("<p>ERROR: {$e->getMessage()}</p>");
        }
        ?>
    </body>
</html>