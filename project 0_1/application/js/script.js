$(window).ready( function() {
    var search_bar = document.getElementById("search_bar");
    search_bar.onkeypress = on_key_press;

    var page_number = document.getElementById("page_number");
    $("#prev").on("click", function() {
        var number = parseInt(page_number.innerText);
        if (number != 1) {
            page_number.innerText = (number - 1).toString();
            var keyword = search_bar.value;
            request_page(number - 1, keyword);
            document.getElementById("next").style.visibility = "visible";
        } else {
            document.getElementById("prev").style.visibility = "hidden";
        }
    });

    $("#next").on("click", function() {
        var number = parseInt(page_number.innerText);
        page_number.innerText = (number + 1).toString();
        var keyword = search_bar.value;
        request_page(number + 1, keyword);

        document.getElementById("prev").style.visibility = "visible";
    });
});

function on_key_press(event) {
    if (is_enter(event)) {
        var keyword = search_bar.value;
        search_sentence(keyword);
    }
}

// keyCode = 13 為 "Enter"
function is_enter(event) {
    return event.keyCode == 13;
}

function search_sentence(str) {
    // Remove previous result.
    $("#table_body").empty();

    $.ajax({
        type : "GET",
        url  : "http://localhost:8080/backend/search_sentence.php",
        data : {keyword : str},
        success : function (response) {
            request_page(1, str);
        },
        error : function (err_msg) {
            insert_server_error_msg();
        }
    });
}

function request_page(number, keyword) {
    $.ajax({
        type : "GET",
        url  : "http://localhost:8080/backend/read_page.php",
        data : {page_number : number},
        success : function (response) {
            var result = JSON.parse(response);
            if (result.length > 0) {
                // Remove previous result.
                $("#table_body").empty();
                insert_table_item(result, keyword);
            } else {
                var page_number = document.getElementById("page_number");
                page_number.innerText = (number - 1).toString();
                document.getElementById("next").style.visibility = "hidden";
            }
        },
        error : function (err_msg) {
            // Remove previous result.
            $("#table_body").empty();
            insert_server_error_msg();
        }
    });
}

function insert_table_item(sentences, keyword) {
    var tr, content, count;

    var body = document.getElementById("table_body");

    sentences.forEach(element => {
        tr = document.createElement("tr");
        
        count = document.createElement("td");
        count.className = "text-center";
        count.innerText = element["count"];

        content = document.createElement("td");
        content.append(
            highlight_keyword(element["content"], keyword)
        );
        
        tr.append(count);
        tr.append(content);
        body.append(tr);
    });
}

function insert_server_error_msg() {
    var body = document.getElementById("table_body");
    var tr = document.createElement("tr");
    
    var count_field = document.createElement("td");
    count_field.innerText = "--";
    count_field.className = "text-center";

    var content_field = document.createElement("td");
    content_field.innerText = "無法連線";
    
    tr.append(count_field);
    tr.append(content_field);
    body.append(tr);
}

function highlight_keyword(content, keyword){
    var new_content = document.createElement("div");
    
    var start = 0;
    var index = content.indexOf(keyword);
    while(index != -1 && index < content.length){
        prefix = document.createElement("span");
        prefix.innerText = content.substr(start, index - start);
        new_content.append(prefix);

        emphasize = document.createElement("em");
        emphasize.innerText = keyword;
        new_content.append(emphasize);

        start = index + keyword.length;
        index = content.indexOf(keyword, start);
    }
    
    // Remaining content.
    if(start < content.length - 1){
        prefix = document.createElement("span");
        prefix.innerText = content.substr(start, content.length - start);
        new_content.append(prefix);
    }
    
    return new_content;
}