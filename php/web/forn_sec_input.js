function adicionarInput() {

    var container = document.getElementById("input_container");

    container.appendChild(document.createTextNode("Fornecedor secundario: "));

    var inputForn = document.createElement("input");

    inputForn.setAttribute('list', 'forn_list');
    inputForn.name = "forn_secundarios[]";
    inputForn.autocomplete = "off";

    container.appendChild(inputForn);

    container.appendChild(document.createTextNode((" Nome: ")));

    var inputName = document.createElement("input");

    inputName.name = "forn_secundarios_nomes[]";
    inputName.autocomplete = "off";

    container.appendChild(inputName);

    container.appendChild(document.createElement("br"));
    container.appendChild(document.createElement("br"));
}