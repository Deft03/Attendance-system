const alertBox = document.getElementById("alert")
const alertMessage = document.getElementById("alert-message")
const books = JSON.parse(localStorage.getItem("books")  || "[]")

updateList()

function addBook() {
    const bookTitle = document.getElementById("bookTitle");
    const bookAuthor = document.getElementById("bookAuthor");
    const bookISBN = document.getElementById("bookISBN");

    // Add book to books array
    books.push({ title: bookTitle.value, author: bookAuthor.value, isbn: bookISBN.value })

    localStorage.setItem("books", JSON.stringify(books));

    // Run add to list
    updateList("Successfully Added The Book!");

    // set value back to nothing
    bookTitle.value = ""
    bookAuthor.value = ""
    bookISBN.value = ""

};


function updateList(message) {
    const bookList = document.getElementById("book-list");


    // Add books to array
    let bookTr = "";

    // Map tru all books
    books.map((book, index) => {
        bookTr += '<tr>' +
            '<th>' + book.title + '</th>' +
            '<th>' + book.author + '</th>' +
            '<th>' + book.isbn + '</th>' +
            '<th> <button onclick="deleteFromList(' + index + ')" class="delete-btn">&times;</button> </th>' +
            '</tr>';
    })

    // Set innerHTML
    bookList.innerHTML = bookTr;

    // Check if there's no message
    if (!message) {
        return alertBox.style.opacity = "0";
    }

    // Show the alert box
    alertBox.style.opacity = "1";
    alertMessage.textContent = message;

    // Set timeout to remove the alert
    setTimeout(() => {
        alertBox.style.opacity = "0"
    }, 3000);

}


function deleteFromList(index) {

    // Delete Book from array
    books.splice(index, 1);

    // Save the books back in localStorage
    localStorage.setItem("books", JSON.stringify(books));


    // Run function to update list
    updateList("Successfully Removed the book!");
}