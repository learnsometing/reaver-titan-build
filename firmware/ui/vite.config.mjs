import { defineConfig } from 'vite'
import { svelte } from '@sveltejs/vite-plugin-svelte'
import { viteSingleFile } from 'vite-plugin-singlefile'
import { fileURLToPath } from 'url'
import path from 'path'

const __dirname = path.dirname(fileURLToPath(import.meta.url))

export default defineConfig({
  root: __dirname,
  plugins: [
    svelte(),
    viteSingleFile(),
    mockEsp32(),
  ],
  build: {
    outDir: path.join(__dirname, 'dist'),
    emptyOutDir: true,
    cssCodeSplit: false,
    assetsInlineLimit: Infinity,
  },
})

function mockEsp32() {
  return {
    name: 'mock-esp32',
    configureServer(server) {
      server.middlewares.use((req, res, next) => {
        const url = new URL(req.url, 'http://localhost')

        if (req.method === 'GET' && url.pathname === '/brightness') {
          const v = url.searchParams.get('v')
          console.log(`[mock] brightness → ${v}`)
          res.writeHead(200, { 'Content-Type': 'text/plain' })
          return res.end('OK')
        }

        if (req.method !== 'POST') return next()

        if (url.pathname === '/zone/head' || url.pathname === '/zone/torso') {
          const zone = url.pathname.split('/')[2]
          const state = url.searchParams.get('state')
          console.log(`[mock] zone/${zone} → ${state}`)
          res.writeHead(200, { 'Content-Type': 'text/plain' })
          return res.end('OK')
        }

        if (url.pathname === '/blaster/state') {
          const state = url.searchParams.get('state')
          console.log(`[mock] blaster/state → ${state}`)
          res.writeHead(200, { 'Content-Type': 'text/plain' })
          return res.end('OK')
        }

        next()
      })
    },
  }
}
