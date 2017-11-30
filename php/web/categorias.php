<html>
    <head>
        <link rel="stylesheet" href="styles.css" type="text/css">
    </head>
    <body>
        <h2>Categorias</h2>
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

            //Creation of the categories tables
            $sql1 = "SELECT * FROM categoria_simples;";
            $sql2 = "SELECT * FROM super_categoria;";


            /*Criacao da tabela das categorias simples*/

            echo("<table cellspacing=\"10\">\n");

                //Table headers para categorias simples
            echo("<tr>
                    <th>Categorias Simples</th>
                  </tr>");

            $result = $db->query($sql1);

                //Insercao das categorias simples como rows na tabela
            foreach($result as $row)
            {
                echo("<tr>
                        <td>{$row['nome']}</td>
                        <td><a href=\"remove_cat.php?nome={$row['nome']}&tipo=simples\">Remover categoria</a></td>\n
                    </tr>");
            }

            echo("</table>\n");

            echo("<p><a href=\"categoria_simples.php\">Nova categoria simples</a></p>
                  <p><a href=\"super_categoria.php\">Nova super categoria </a></p>");

            $url = htmlspecialchars($_SERVER['HTTP_REFERER']);
            echo "<a href='$url'>Voltar</a>";


            /*Criacao da tabela das super categorias*/

            echo("<table cellspacing=\"10\" id = 'super_cat'>\n");

            //Table headers para super categorias
            echo("<tr>
                    <th>Super Categorias</th>
                  </tr>");

            $result = $db->query($sql2);

                //Insercao das super categorias como rows na tabela
            foreach($result as $row)
            {
                echo("<tr>
                        <td>{$row['nome']}</td>
                        <td><a href=\"listar_categoria.php?nome={$row['nome']}\">Listar sub-categorias</a></td>\n
                        <td><a href=\"remove_cat.php?nome={$row['nome']}&tipo=super\">Remover categoria</a></td>
                    </tr>");
            }

            echo("</table>\n");

        }
        catch (PDOException $e)
        {
            echo("<p>ERROR: {$e->getMessage()}</p>");
        }
        ?>
    </body>
</html>