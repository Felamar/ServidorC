// Get the movie title from url
const urlParams = new URLSearchParams(window.location.search);
const movieTitle = urlParams.get("movie");
let checkout_json = "";
let seatsSelected = [];
let showtimes = [];
const xhr = new XMLHttpRequest();
xhr.open("GET", "http://localhost:5000/movies/" + movieTitle);

function markSelectedSeatsAsOccupied() {
  const selectedSeats = document.querySelectorAll(".seat.selected");
  for (let i = 0; i < selectedSeats.length; i++) {
    selectedSeats[i].classList.remove("selected");
    selectedSeats[i].classList.add("occupied");
    selectedSeats[i].removeEventListener("click",() => {
      toggleSeatState(seat);
      updateSelectedCount();
    });
  }
  updateSelectedCount();
}

function updateSelectedCount() {
  const selectedSeats = document.querySelectorAll(".seat.selected");
  const count = selectedSeats.length;
  document.querySelector(".selected-seats-count").textContent = count;
  document.querySelector(".selected-seats").textContent = seatsSelected.join(", ");
}

const xhr_2 = new XMLHttpRequest();
xhr_2.onload = function () {
  console.log("Response received");
  if (xhr_2.status === 200) {
    console.log("Checkout successful");
    alert("Checkout successful");
    const movieData = JSON.parse(xhr_2.responseText);
    showtimes = movieData[0].showtimes;
    console.log(showtimes); 
    seatsSelected = [];
    markSelectedSeatsAsOccupied();

  } else if (xhr_2.status !== 200) {
    console.log("Request failed.  Returned status of " + xhr_2.status);
    alert("Checkout failed");
  }
};



xhr.onload = () => {
  console.log("Información de películas obtenida");
  const movieInfo = document.querySelector(".movie-info");
  const seatsMap = document.querySelector(".seat-map");
  const select = document.getElementById("showtime-select");
  const checkoutBtn = document.querySelector("#checkout-btn");
  checkoutBtn.addEventListener("click", checkout);


  while (movieInfo.firstChild) {
    movieInfo.removeChild(movieInfo.firstChild);
  }
  while (seatsMap.firstChild) {
    seatsMap.removeChild(seatsMap.firstChild);
  }

  const movieData = JSON.parse(xhr.responseText);

  const { title, description} = movieData[0];
  showtimes = movieData[0].showtimes;
  console.log(title, description, showtimes);

  if (title.trim() !== "") {
    const movie = document.createElement("div");
    const select = document.getElementById("showtime-select");
    movie.classList.add("movie");
    movie.innerHTML = `
      <h2 class="movie-title">${title}</h2>
      <img class="movie-poster" src="../media/${encodeURIComponent(
        title
      )}.jpg" alt="${title} Poster"/>
      <p class="movie-description">${description}</p>
      `;
    movieInfo.appendChild(movie);

    for (let i = 0; i < showtimes.length; i++) {
      const option = document.createElement("option");
      option.text = showtimes[i].time;
      select.add(option);
    }
  }

  function toggleSeatState(seat) {
    if (seat.classList.contains("available")) {
      seat.classList.remove("available");
      seat.classList.add("selected");
      seatsSelected.push(seat.querySelector(".seat-number").textContent);
    } else if (seat.classList.contains("selected")) {
      seat.classList.remove("selected");
      seat.classList.add("available");
      seatsSelected.pop(seat.querySelector(".seat-number").textContent);
    }
  }

  displaySeats(showtimes[0].time);

  function displaySeats(selectedShowtime) {
    console.log(`Selected showtime: ${selectedShowtime}`);
    let index = 0;
    for (let i = 0; i < showtimes.length; i++) {
      if (showtimes[i].time === selectedShowtime) {
        index = i;
      }
    }
    while (seatsMap.firstChild) {
      seatsMap.removeChild(seatsMap.firstChild);
    }
    console.log(`Showtimes`);
    console.log(showtimes[index].seats);
    for (let i = 0; i < showtimes[index].seats.length; i += 5) {
      const row = document.createElement("div");
      row.classList.add("row");
      for (let j = i; j < i + 5 && j < showtimes[index].seats.length; j++) {
        const seat = document.createElement("div");
        seat.classList.add("seat");
        if (showtimes[index].seats[j] == 1) {
          seat.classList.add("available");
          seat.addEventListener("click", () => {
            toggleSeatState(seat);
            updateSelectedCount();
          });
        } else {
          seat.classList.add("occupied");
        }
        seat.innerHTML = `
          <span class="seat-number">${j < 5 ? "A" : "B"}${j >= 5 ? j - 4 : j + 1}</span>
        `;
        row.appendChild(seat);
      }

      seatsMap.appendChild(row);
    }
    seatsSelected = [];
    updateSelectedCount();
  }

  select.addEventListener("change", (event) => {
    const selectedOption = event.target.selectedOptions[0];
    const selectedShowtime = selectedOption.value;
    displaySeats(selectedShowtime);
  });

  function checkout() {
    const selectedSeats = document.querySelectorAll(".seat.selected");
    const totalSelectedSeats = selectedSeats.length;
    if (totalSelectedSeats === 0) {
      alert("You have not selected any seats");
      return;
    }
    const confirmation = confirm(`You have selected ${totalSelectedSeats} seats: ` +
                                `${seatsSelected.join(", ")}\n\n` +
                                `The total cost is $${totalSelectedSeats * 10}\n\n` +
                                `Do you want to proceed to checkout?`);
    if (confirmation) {
      checkout_json = movieTitle + "/" + select.value + "/" + seatsSelected.join(",");
      xhr_2.open("POST", "http://localhost:5000/checkout/" + checkout_json);
      xhr_2.send();
    }
  }
};

/* Se muestra un mensaje de error en la consola si la petición falla */
xhr.onerror = (error) => {
  console.error("XHR error:", error);
};

/* Se muestra un mensaje de error en la consola si la petición falla */
xhr_2.onerror = (error) => {
  console.error("XHR 2 error:", error);
};

xhr.send();