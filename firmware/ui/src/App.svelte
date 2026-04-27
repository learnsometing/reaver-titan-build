<script>
  let headOn = $state(false)
  let torsoOn = $state(false)
  let blasterState = $state('idle')
  let view = $state('overview')
  let brightness = $state(0)

  function setBrightness(v) {
    brightness = Number(v)
    fetch(`/brightness?v=${brightness}`)
  }

  async function setZone(zone, on) {
    if (zone === 'head') headOn = on
    else torsoOn = on
    await fetch(`/zone/${zone}?state=${on ? 'on' : 'off'}`, { method: 'POST' })
  }

  async function setBlaster(state) {
    blasterState = state
    await fetch(`/blaster/state?state=${state}`, { method: 'POST' })
  }

  const blasterStates = ['idle', 'prime', 'charge', 'discharge']

  const stateLabel = {
    idle: 'IDLE',
    prime: 'PRIME',
    charge: 'FULL CHARGE',
    discharge: 'DISCHARGE',
  }
</script>

<div class="scanlines"></div>

<main>
  <header>
    <h1 class="glitch" data-text="REAVER TITAN">REAVER TITAN</h1>
    <p class="subtitle">MACHINE SPIRIT INTERFACE — CHAOS MARK IV</p>
  </header>

  {#if view === 'overview'}
    <section class="zones">
      <h2 class="section-title">// ZONE STATUS</h2>
      <div class="zone-grid">
        <div class="zone-card">
          <span class="zone-label">HEAD</span>
          <button
            class="toggle"
            class:active={headOn}
            onclick={() => setZone('head', !headOn)}
          >
            {headOn ? 'ONLINE' : 'OFFLINE'}
          </button>
        </div>
        <div class="zone-card">
          <span class="zone-label">TORSO</span>
          <button
            class="toggle"
            class:active={torsoOn}
            onclick={() => setZone('torso', !torsoOn)}
          >
            {torsoOn ? 'ONLINE' : 'OFFLINE'}
          </button>
        </div>
      </div>
    </section>

    <section class="brightness-section">
      <h2 class="section-title">// LED BRIGHTNESS</h2>
      <div class="slider-row">
        <input
          type="range"
          min="0"
          max="4095"
          value={brightness}
          oninput={(e) => setBrightness(e.target.value)}
          class="brightness-slider"
        />
        <span class="brightness-val">{brightness}</span>
      </div>
    </section>

    <section class="weapons">
      <h2 class="section-title">// WEAPONS</h2>
      <button class="weapon-card" onclick={() => (view = 'blaster')}>
        <span class="weapon-name">LASER BLASTER</span>
        <span class="weapon-state">STATE: {stateLabel[blasterState]}</span>
        <span class="weapon-nav">[ TAP TO CONTROL → ]</span>
      </button>
    </section>
  {/if}

  {#if view === 'blaster'}
    <section class="blaster-view">
      <button class="back-btn" onclick={() => (view = 'overview')}>← BACK</button>
      <h2 class="section-title">// LASER BLASTER</h2>
      <p class="current-state">CURRENT STATE: <span class="highlight">{stateLabel[blasterState]}</span></p>
      <div class="state-grid">
        {#each blasterStates as s}
          <button
            class="state-btn"
            class:active={blasterState === s}
            onclick={() => setBlaster(s)}
          >
            {stateLabel[s]}
          </button>
        {/each}
      </div>
    </section>
  {/if}
</main>

<style>
  :global(*, *::before, *::after) {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
  }

  :global(body) {
    background: #000;
    color: #00ff41;
    font-family: 'Courier New', Courier, monospace;
    min-height: 100vh;
    overflow-x: hidden;
  }

  .scanlines {
    position: fixed;
    inset: 0;
    background: repeating-linear-gradient(
      0deg,
      transparent,
      transparent 2px,
      rgba(0, 0, 0, 0.12) 2px,
      rgba(0, 0, 0, 0.12) 4px
    );
    pointer-events: none;
    z-index: 100;
  }

  main {
    max-width: 480px;
    margin: 0 auto;
    padding: 2rem 1.25rem 4rem;
  }

  header {
    text-align: center;
    margin-bottom: 2.5rem;
    padding-bottom: 1.5rem;
    border-bottom: 1px solid #003300;
  }

  h1.glitch {
    font-size: 1.75rem;
    letter-spacing: 0.25em;
    text-transform: uppercase;
    color: #00ff41;
    position: relative;
    animation: glitch 4s infinite;
  }

  h1.glitch::before,
  h1.glitch::after {
    content: attr(data-text);
    position: absolute;
    top: 0;
    left: 0;
    width: 100%;
  }

  h1.glitch::before {
    color: #ff0040;
    animation: glitch-top 4s infinite;
    clip-path: polygon(0 0, 100% 0, 100% 35%, 0 35%);
  }

  h1.glitch::after {
    color: #00eaff;
    animation: glitch-bottom 4s infinite;
    clip-path: polygon(0 65%, 100% 65%, 100% 100%, 0 100%);
  }

  @keyframes glitch {
    0%, 90%, 100% { transform: none; }
    91% { transform: skewX(1deg); }
    93% { transform: skewX(-1deg); }
    95% { transform: none; }
  }

  @keyframes glitch-top {
    0%, 90%, 100% { transform: none; opacity: 0; }
    91% { transform: translate(-2px, -1px); opacity: 0.8; }
    93% { transform: translate(2px, 1px); opacity: 0.8; }
    95% { transform: none; opacity: 0; }
  }

  @keyframes glitch-bottom {
    0%, 90%, 100% { transform: none; opacity: 0; }
    91% { transform: translate(2px, 1px); opacity: 0.8; }
    93% { transform: translate(-2px, -1px); opacity: 0.8; }
    95% { transform: none; opacity: 0; }
  }

  .subtitle {
    font-size: 0.6rem;
    letter-spacing: 0.15em;
    color: #005500;
    margin-top: 0.5rem;
  }

  .section-title {
    font-size: 0.75rem;
    letter-spacing: 0.1em;
    color: #007700;
    margin-bottom: 1rem;
    font-weight: normal;
  }

  .zones {
    margin-bottom: 2rem;
  }

  .zone-grid {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 0.75rem;
  }

  .zone-card {
    border: 1px solid #003300;
    padding: 1rem;
    display: flex;
    flex-direction: column;
    gap: 0.75rem;
    align-items: center;
  }

  .zone-label {
    font-size: 0.7rem;
    letter-spacing: 0.15em;
    color: #007700;
  }

  .toggle {
    background: transparent;
    border: 1px solid #005500;
    color: #005500;
    font-family: inherit;
    font-size: 0.75rem;
    letter-spacing: 0.1em;
    padding: 0.4rem 0.75rem;
    cursor: pointer;
    transition: all 0.15s;
    width: 100%;
  }

  .toggle:hover {
    border-color: #00ff41;
    color: #00ff41;
  }

  .toggle.active {
    border-color: #00ff41;
    color: #00ff41;
    background: rgba(0, 255, 65, 0.08);
    text-shadow: 0 0 8px #00ff41;
  }

  .brightness-section {
    margin-bottom: 2rem;
  }

  .slider-row {
    display: flex;
    align-items: center;
    gap: 1rem;
  }

  .brightness-slider {
    flex: 1;
    accent-color: #00ff41;
    cursor: pointer;
  }

  .brightness-val {
    font-size: 0.75rem;
    color: #00ff41;
    min-width: 2.5rem;
    text-align: right;
  }

  .weapons {
    margin-bottom: 2rem;
  }

  .weapon-card {
    width: 100%;
    background: transparent;
    border: 1px solid #003300;
    color: #00ff41;
    font-family: inherit;
    padding: 1.25rem;
    cursor: pointer;
    text-align: left;
    display: flex;
    flex-direction: column;
    gap: 0.4rem;
    transition: border-color 0.15s;
  }

  .weapon-card:hover {
    border-color: #00ff41;
    background: rgba(0, 255, 65, 0.04);
  }

  .weapon-name {
    font-size: 0.85rem;
    letter-spacing: 0.1em;
  }

  .weapon-state {
    font-size: 0.65rem;
    color: #007700;
    letter-spacing: 0.08em;
  }

  .weapon-nav {
    font-size: 0.6rem;
    color: #005500;
    margin-top: 0.25rem;
  }

  .blaster-view {
    display: flex;
    flex-direction: column;
    gap: 1.25rem;
  }

  .back-btn {
    background: transparent;
    border: none;
    color: #007700;
    font-family: inherit;
    font-size: 0.75rem;
    letter-spacing: 0.1em;
    cursor: pointer;
    text-align: left;
    padding: 0;
  }

  .back-btn:hover {
    color: #00ff41;
  }

  .current-state {
    font-size: 0.75rem;
    color: #007700;
    letter-spacing: 0.08em;
  }

  .highlight {
    color: #00ff41;
    text-shadow: 0 0 8px #00ff41;
  }

  .state-grid {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 0.75rem;
  }

  .state-btn {
    background: transparent;
    border: 1px solid #003300;
    color: #005500;
    font-family: inherit;
    font-size: 0.75rem;
    letter-spacing: 0.12em;
    padding: 1rem 0.5rem;
    cursor: pointer;
    transition: all 0.15s;
  }

  .state-btn:hover {
    border-color: #00ff41;
    color: #00ff41;
  }

  .state-btn.active {
    border-color: #00ff41;
    color: #00ff41;
    background: rgba(0, 255, 65, 0.08);
    text-shadow: 0 0 8px #00ff41;
  }
</style>
