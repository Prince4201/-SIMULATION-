# ATMVerse Features

## Customer Features 👤
- **Virtual ATM Card**: Unique 16-digit card number, CVV, expiry, hashed PIN.
- **Secure Login**: PIN masking (dots), 3-attempt lockout, session timeout.
- **Dashboard/Home**: View available balance and daily limits.
- **Cash Withdrawal**: Fast cash options + custom amount entry. Validates ATM cash limits and customer daily limits.
- **Cash Deposit**: Note simulation, updates ATM cash reserves.
- **Fund Transfer**: Transfer money instantly to another account using their Account Number.
- **Mini Statement**: View the last 7 transactions in a formatted table.
- **Transaction History**: View full scrollable transaction history.
- **Card Settings**: Change PIN securely, view card details (rendered as ASCII art).
- **Profile**: View customer and account details.

## Admin Features 🛡️
- **Admin Authentication**: Separate secure login flow.
- **Dashboard**: Central control panel.
- **System Reports**: Daily transaction aggregates, total withdrawals/deposits, most active user detection.
- **Cash Management**: View ATM cash reserves visually using an ASCII Bar Graph. Refill functionality.
- **Log Auditing**: System tracks all logins, transfers, PIN changes, and locks.

## UI/UX Features 🎨
- **Immersive ATM Frame**: The entire application runs inside a beautifully rendered, rounded ATM border.
- **Hardware Animations**:
  - Card sliding in and out.
  - Cash being counted (`*click* *whir*`).
  - Cash dispensing out of the slot.
  - Receipt paper printing line-by-line.
- **Dynamic Feedback**: Loading spinners (`⠋⠙⠹`), processing dots (`...`), and colored success/error alerts.
- **Theming**: Premium neon-blue and dark gray aesthetics.

## Technical Features ⚙️
- **Zero Database Setup**: Fully runs on local `.json` files.
- **Greedy Cash Algorithm**: Intelligently dispenses the largest possible notes.
- **Transactional Safety**: If cash fails to dispense, the account balance deduction is rolled back.
- **Cross Platform UI**: Uses standard ANSI codes supported by modern Windows, macOS, and Linux terminals.
