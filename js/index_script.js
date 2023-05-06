const xhr = new XMLHttpRequest();
xhr.open("GET", "http://localhost:5000/movies");
xhr.onload = () => {
  console.log("Received movie data from server");
  const movieList = document.querySelector(".movie-list");

  // Remove existing movie elements from list
  while (movieList.firstChild) {
    movieList.removeChild(movieList.firstChild);
  }

  // Parse movie data from server response
  const movieData = JSON.parse(xhr.responseText);

  // Create new movie element for each title
  movieData.forEach((data) => {
    const { title, description } = data;

    if (title.trim() !== "") {
      const movie = document.createElement("div");
      movie.classList.add("movie");
      movie.innerHTML = `
        <img
          src="../media/${encodeURIComponent(title)}.jpg"
          alt="${title} Poster"
        />
        <div class="movie-details">
          <h2 class="movie-title">${title}</h2>
          <p class="movie-desc">${description.substr(0, 100)}...</p>
          <a href="booking.html?movie=${encodeURIComponent(title)}" class="movie-btn">Book Now</a>
        </div>
        `;
      movieList.appendChild(movie);
    }
  });
};
xhr.onerror = (error) => {
  console.error("XHR error:", error);
};
xhr.send();
