<html>
    <link rel="stylesheet" href="styles.css" type="text/css">
    <body>
        <h2>Categorias</h2>
        <?php
        try
        {
            //Initializing connection
            $host = "db.ist.utl.pt";
            $user ="ist157842";
            $password = "kikoki";
            $dbname = $user;

            $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
            $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            //Creation of the categories tables
            $sql1 = "SELECT * FROM categoria_simples;";
            $sql2 = "SELECT * FROM super_categoria;";
            $result = $db->query($sql1);

            echo("<table cellspacing=\"10\">\n");

                //Table headers
            echo("<tr>\n
                    <th>Categorias Simples</th>\n
                  </tr>\n");

                //Table content
            foreach($result as $row)
            {
                echo("<tr>\n
                        <td>{$row['nome']}</td>\n
                    </tr>\n<p>");
            }
//esta' feio:
// FIXME
            $result = $db->query($sql2);
            echo("<tr>\n
                    <th>Super Categorias</th>\n
                  </tr>\n");

                //Table content
            foreach($result as $row)
            {
                echo("<tr>\n
                        <td>{$row['nome']}</td>\n
                        <td><a href=\"listar_categoria.php?nome={$row['nome']}\">Listar sub-categorias</a></td>\n
                    </tr>\n");
            }

            echo("</table>\n");
            echo("<tr><td><a href=\"categoria_simples.php\">Nova categoria simples</a></td></tr>");
            echo("<p><tr><td><a href=\"super_categoria.php\">Nova supercategoria </a></td></tr>");

        }
        catch (PDOException $e)
        {
            echo("<p>ERROR: {$e->getMessage()}</p>");
        }
        ?>
    </body>
</html>