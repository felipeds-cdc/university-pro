document.addEventListener("DOMContentLoaded", () => {
  const cards = document.querySelectorAll(".news-card");
  const prevButton = document.querySelector(".carousel-prev");
  const nextButton = document.querySelector(".carousel-next");

  let currentIndex = 0;

  const updateCarousel = () => {
    cards.forEach((card, index) => {
      card.classList.toggle("active", index === currentIndex);
    });
  };

  prevButton.addEventListener("click", () => {
    currentIndex = (currentIndex - 1 + cards.length) % cards.length;
    updateCarousel();
  });

  nextButton.addEventListener("click", () => {
    currentIndex = (currentIndex + 1) % cards.length;
    updateCarousel();
  });

  // Iniciar com o primeiro cartão visível
  updateCarousel();
});
