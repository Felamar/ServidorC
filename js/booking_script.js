

// Get all the seats
const seats = document.querySelectorAll('.seat');

// Get the count and total elements
const count = document.querySelector('.count');
const selectedSeats = document.querySelector('.selected-seats');
const selectedRows = document.querySelector('.selected-rows');
const selectedCols = document.querySelector('.selected-cols');

// Initial count and total
let total = 0;

// Function to update the selected seats and the count
function updateSelectedCount() {
  const selected = document.querySelectorAll('.seat.selected');

  // Copy selected seats into an array
  const selectedSeatsArray = [...selected];

  // Map through the array
  const seatNumbersArray = selectedSeatsArray.map((seat) => {
    return `${seat.dataset.row}${seat.dataset.col}`;
  });

  // Update the selected seats, rows, and columns
  selectedSeats.textContent = seatNumbersArray.join(', ');

  // Update the count
  total = selectedSeatsArray.length;
  count.textContent = total;
}

// Event listener for the seats
seats.forEach((seat) => {
  seat.addEventListener('click', () => {
    seat.classList.toggle('selected');
    updateSelectedCount();
  });
});

// get the movies titles and descriptions from socketServer.c
const movieDescription = JSON.parse(document.querySelector('.movie-description').textContent);


// Get the movie title from url
const urlParams = new URLSearchParams(window.location.search);
const movieTitle = urlParams.get('movie');

document.querySelector('.movie-title').textContent = movieTitle;
document.querySelector('.movie-description').textContent = movieDescription[movieTitle];
// update the poster image and its alt from the media folder
document.querySelector('.movie-poster').src = `../media/${movieTitle}.jpg`;
document.querySelector('.movie-poster').alt = `${movieTitle} poster`;




  