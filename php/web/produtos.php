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
            echo("<tr>
                    <th>EAN</th>
                    <th>Designacao</th>
                    <th>Categoria</th>
                    <th>Fornecedor primario</th>
                    <th>Data</th>
                  </tr>");

                //Table content
            foreach($result as $row)
            {
                echo("<tr>\n
                        <td>{$row['ean']}</td>
                        <td>{$row['design']}</td>
                        <td>{$row['categoria']}</td>
                        <td>{$row['forn_primario']}</td>
                        <td>{$row['data']}</td>
                        <td><a href=\"design_d.php?ean={$row['ean']}\">Editar designacao</a></td>
                        <td><a href=\"update_prod.php?ean={$row['ean']}&tipo=remover\">Remover produto</a></td>
                        <td><a href=\"listar_reposicoes.php?ean={$row['ean']}\">Listar eventos de reposicao</a></td>
                    </tr>");
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