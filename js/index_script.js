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
        <h2>${title}</h2>
        <p>${description}</p>
        <a href="booking.html?movie=${encodeURIComponent(
          title
        )}" class="btn">Book Now</a>
      `;
      movieList.appendChild(movie);
    }
  });
};
xhr.onerror = (error) => {
  console.error("XHR error:", error);
};
xhr.send();
