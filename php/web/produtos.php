<html>
    <link rel="stylesheet" href="styles.css" type="text/css">
    <body>
        <h2>Produtos</h2>
        <?php
        try
        {
            //Initializing connection
            $host = "db.ist.utl.pt";
            $user ="ist426008";
            $password = "northernlights";
            $dbname = $user;

            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            //Creation of the products table
            $sql = "SELECT * FROM produto;";
            $result = $db->query($sql);

            echo("<table cellspacing=\"10\">\n");

                //Table headers
            echo("<tr>\n
                    <th>EAN</th>\n
                    <th>Designacao</th>\n
                    <th>Categoria</th>\n
                    <th>Fornecedor primario</th>\n
                    <th>Data</th>\n
                  </tr>\n");

                //Table content
            foreach($result as $row)
            {
                echo("<tr>\n
                        <td>{$row['ean']}</td>\n
                        <td>{$row['design']}</td>\n
                        <td>{$row['categoria']}</td>\n
                        <td>{$row['forn_primario']}</td>\n
                        <td>{$row['data']}</td>\n
                        <td><a href=\"design_d.php?ean={$row['ean']}\">Editar designacao</a></td>\n
                        <td><a href=\"update_prod.php?ean={$row['ean']}&tipo=remover\">Remover produto</a></td>\n
                        <td><a href=\"listar_reposicoes.php?ean={$row['ean']}\">Listar eventos de reposicao</a></td>\n
                    </tr>\n");
            }

            echo("</table>\n");
            echo("<tr><td><a href=\"inserir_produto_b.php\">Novo produto</a></td></tr>");

        }
        catch (PDOException $e)
        {
            echo("<p>ERROR: {$e->getMessage()}</p>");
        }
        ?>
    </body>
</html>