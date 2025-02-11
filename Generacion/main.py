from diffusers import StableDiffusionPipeline
import torch

# Cargar el modelo de Stable Diffusion
pipe = StableDiffusionPipeline.from_pretrained("stabilityai/stable-diffusion-2-1-base")

pipe.to("cpu")

# Generar la imagen con un tamaño más pequeño, por ejemplo 256x256
prompt = "A cute cartoon dog"
image = pipe(prompt, height=256, width=256).images[0]

# Mostrar la imagen generada
image.show()
